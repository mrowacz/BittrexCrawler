import re
import sys
import json
import requests
import logging

import datetime
from functional import seq
from pymongo import MongoClient

class BittrexCrawler(object):
    def __init__(self, debug_level=logging.DEBUG):
        # logger
        root = logging.getLogger()
        logging.basicConfig(filename='bittrex.log', filemode ='a', level=debug_level)
        ch = logging.StreamHandler(sys.stdout)
        ch.setLevel(logging.DEBUG)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        ch.setFormatter(formatter)
        root.addHandler(ch)
        # mongo
        self.client = MongoClient()

        logging.info("BittrexCrawler started ...")
    def get_markets(self):
        url="https://bittrex.com/api/v1.1/public/getmarkets"
        r = requests.get(url)
        if r.status_code == 200:
            self.analyze_market_response(r.text)
        else:
            logging.error('Cannot read markets url!')
    def analyze_market_response(self, data):
        js = json.loads(data)
        if js['success'] is True:

            db = self.client['config']
            markets = db['markets']

            mongo_markets = set()
            for market in markets.find():
                mongo_markets.add(market["market"])
                logging.info(market)

            markets_data  = seq(js['result'])\
                .filter(lambda x: x['BaseCurrency'] == 'BTC' or x['BaseCurrency'] == 'ETH')\
                .map(lambda x: x['MarketName'])\
                .filter(lambda x: x not in mongo_markets)

            if not markets_data.empty():
                for entry in markets_data:
                    logging.info("Inserting new entry -- " + entry)
                    markets.insert({"market":entry})
            else:
                logging.info("No new market")

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
        url="https://bittrex.com/api/v1.1/public/getticker?market=" + name

        r = requests.get(url)
        data = self.analyze_ticker(r)

        if data is None:
            return ""

        cursor_data = {}
        cursor_data["date"] =  datetime.datetime.now()
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
        

    # main function which is the heart of this script
    def fetch_btc_market(self):
        # first download and update market list
        self.get_markets()

        # get market list
        market_list = []
        db = self.client['config']
        markets = db['markets']

        market_list = seq(markets.find())\
            .map(lambda obj : obj["market"])\
            .filter(lambda str: re.match("^BTC-", str))

        logging.debug("MARKET LIST=" + str(market_list))

        # get all currencies values
        db_hist = self.client['history']
        cursor = db_hist["hist"]

        for market in market_list:
            logging.info("Fetching - " + market)

            last_price_curr = cursor.find({"market": market}).limit(1)
            curr_price = self.fetch_market(market)

            if curr_price == '':
                logging.error("Cannot get data for market=" + markets)
                continue

            last_price = None
            for obj in last_price_curr:
                last_price = obj

            if last_price is None:
                post = {
                    "market": market,
                    "Bid": curr_price["Bid"],
                    "Ask": curr_price["Ask"],
                    "Last": curr_price["Last"]
                }
                db_hist["hist"].insert(post)
                logging.info("Creating new last record for " + market + " -- " + str(post))
            else:
                logging.error("Not Empty " + str(last_price))


            # print last_price, curr_price
            break
        # print self.fetch_market(market_list[0])




if __name__ == "__main__":
    c = BittrexCrawler(debug_level=logging.DEBUG)
    # c.get_markets()
    # print c.fetch_market("BTC-ETH")
    # print c.list_market("BTC-ETH")
    c.fetch_btc_market()