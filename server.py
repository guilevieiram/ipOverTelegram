from flask import Flask, request
import requests

app = Flask(__name__)

COUNTER = 0
@app.route("/")
def home():
    global COUNTER
    COUNTER += 1
    return f"Hello counter {COUNTER}"

@app.route("/getUrl", methods=["GET"])
def get_url():
    url = request.args.get("url")
    res = requests.get(url)
    return res.json()

if __name__=="__main__":
    app.run(host="10.0.0.2", port="8000")

