import sys
import requests

API_KEY = "5cd9421accdf6fe2f3eb49d6f901e9c4c8cf1974bcec8f574c6d633dff29c472"


def main():
    if len(sys.argv) != 2:
        print("Missing command-line argument")
        sys.exit(1)

    try:
        amount = float(sys.argv[1])
    except ValueError:
        print("Command-line argument is not a number")
        sys.exit(1)

    price = get_price()
    total = amount * price
    print(f"${total:,.4f}")


def get_price():
    try:
        response = requests.get(f"https://rest.coincap.io/v3/assets/bitcoin?apiKey={API_KEY}")
        response.raise_for_status()
        data = response.json()
        return float(data["data"]["priceUsd"])
    except (requests.RequestException, KeyError, ValueError):
        print("Error fetching Bitcoin price.")
        sys.exit(1)



if __name__ == "__main__":
    main()