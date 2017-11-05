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
                print (market)

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
            logging.error('Error during parsing ' + data + " ticker")

    def fetch_market(self, name=None):
        url="https://bittrex.com/api/v1.1/public/getticker?market=" + name
        db = self.client['data']
        cursor = db[name]

        r = requests.get(url)
        data = self.analyze_ticker(r)
        cursor_data = {}
        cursor_data["date"] =  datetime.datetime.now()
        cursor_data["ask"] = data["Ask"]
        cursor_data["bid"] = data["Bid"]
        cursor_data["last"] = data["Last"]

        cursor.insert(cursor_data)
        return data

    def list_market(self, name):
        db = self.client['data']
        cursor = db[name]

        result = []
        for elem in cursor.find():
            result.append(elem)

        return result

if __name__ == "__main__":
    c = BittrexCrawler()
    #c.get_markets()
    c.fetch_market("BTC-ETH")
    c.list_market("BTC-ETH")