import re
import os
import sys
import json
import time
import logging
import requests
import datetime
from functional import seq
from pymongo import MongoClient


class BittrexCrawler(object):
    def __init__(self, debug_level=logging.DEBUG):
        # logger
        root = logging.getLogger()
        logging.basicConfig(filename='bittrex.log', filemode='a', level=debug_level)
        ch = logging.StreamHandler(sys.stdout)
        ch.setLevel(logging.DEBUG)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        ch.setFormatter(formatter)
        root.addHandler(ch)
        # mongo
        self.client = MongoClient()

        logging.info("BittrexCrawler started ...")

    def get_markets(self):
        url = "https://bittrex.com/api/v1.1/public/getmarkets"
        r = requests.get(url)
        if r.status_code == 200:
            self.analyze_market_response(r.text)
        else:
            logging.error('Cannot read markets url! ' + url)

    def analyze_market_response(self, data):
        js = json.loads(data)
        if js['success'] is True:

            db = self.client['config']
            markets = db['markets']

            mongo_markets = set()
            for market in markets.find():
                mongo_markets.add(market["market"])
                logging.debug(market)

            markets_data = seq(js['result']) \
                .filter(lambda x: x['BaseCurrency'] == 'BTC' or x['BaseCurrency'] == 'ETH') \
                .map(lambda x: x['MarketName']) \
                .filter(lambda x: x not in mongo_markets)

            if not markets_data.empty():
                for entry in markets_data:
                    logging.debug("Inserting new entry -- " + entry)
                    markets.insert({"market": entry})
            else:
                logging.debug("No new market")

        else:
            print js['success']
            logging.error("marker response not successfull " + js['success'])

    def analyze_ticker(self, resp):
        if resp.status_code == 200:
            js = json.loads(resp.text)
            if js['success'] == True:
                return js['result']
        else:
            logging.error('Error during parsing ' + resp.text + " ticker")

    def fetch_market(self, name=None):
        url = "https://bittrex.com/api/v1.1/public/getticker?market=" + name

        r = requests.get(url)
        data = self.analyze_ticker(r)

        if data is None:
            return ""

        cursor_data = {}
        cursor_data["date"] = datetime.datetime.now()
        cursor_data["ask"] = data["Ask"]
        cursor_data["bid"] = data["Bid"]
        cursor_data["last"] = data["Last"]

        return data

    # get all data for selected market - just read data from mongodb
    def list_market(self, name):
        db = self.client['bittrex']
        cursor = db[name]

        result = []
        for elem in cursor.find():
            result.append(elem)

        return result

    def add_record(self, name=None, price=None):
        db = self.client['bittrex']
        cursor = db[name]

        post_data = {
            "date": datetime.datetime.now(),
            "Ask": price["Ask"],
            "Bid": price["Bid"],
            "Last": price["Last"]
        }

        logging.debug("Add record=" + name + ": " + str(price))
        cursor.insert_one(post_data)

    def add_hist_record(self, name=None, price=None):
        db_hist = self.client['history']
        cursor = db_hist["hist"]
        post = {
            "market": name,
            "Bid": price["Bid"],
            "Ask": price["Ask"],
            "Last": price["Last"]
        }
        _id = None
        for data in cursor.find({"market": name}):
            _id = data["_id"]

        if _id is None:
            cursor.insert_one(post)
        else:
            cursor.update_one({"_id": _id}, {"$set": post}, False)
            logging.debug("Updated record: " + str(post))

    # main function which is the heart of this script
    def fetch_btc_market(self):
        # first download and update market list
        self.get_markets()

        # get market list
        db = self.client['config']
        markets = db['markets']

        market_list = seq(markets.find()) \
            .map(lambda obj: obj["market"]) \
            .filter(lambda str: re.match("^BTC-", str))

        logging.debug("MARKET LIST=" + str(market_list))

        # get all currencies values
        db_hist = self.client['history']
        cursor = db_hist["hist"]

        failed = 0
        success = 0
        updated = 0
        skipped = 0
        for market in market_list:
            logging.debug("Fetching - " + market)

            last_price_curr = cursor.find({"market": market}).limit(1)
            curr_price = self.fetch_market(market)

            if curr_price == '':
                failed += 1
                logging.error("Cannot get data for market=" + market)
                continue

            last_price = None
            for obj in last_price_curr:
                last_price = {
                    "Bid": float(obj["Bid"]),
                    "Ask": float(obj["Ask"]),
                    "Last": float(obj["Last"])
                }

            if last_price is None:
                self.add_hist_record(market, curr_price)
                self.add_record(market, curr_price)
            else:
                if last_price["Bid"] != curr_price["Bid"] or \
                                last_price["Ask"] != curr_price["Ask"] or \
                                last_price["Last"] != curr_price["Last"]:
                                    self.add_record(market, curr_price)
                                    self.add_hist_record(market, curr_price)
                                    logging.debug("Ask " + str(last_price["Ask"]) + " " + str(curr_price["Ask"]))
                                    logging.debug("Bid " + str(last_price["Bid"]) + " " + str(curr_price["Bid"]))
                                    logging.debug("Last " + str(last_price["Last"]) + " " + str(curr_price["Last"]))
                                    updated += 1
                else:
                    skipped += 1
                    logging.debug(market + " doesn't changed")
            success += 1
        logging.info("End OK: {0}, FAILED: {1}".format(success, failed))
        logging.info("Updated {0}, SKIPPED: {1}".format(updated, skipped))


if __name__ == "__main__":

    working_dir = "/opt/BittrexCrawler/"
    os.chdir(working_dir)

    c = BittrexCrawler(debug_level=logging.INFO)
    start = time.time()
    c.fetch_btc_market()
    end = time.time()
    logging.info("Time consumed " + '{:6.2f}'.format(end - start))
