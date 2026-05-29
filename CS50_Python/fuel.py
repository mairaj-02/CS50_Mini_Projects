def main():
    while True:
        try:
            user_input = input("Fraction: ")
            decimal = convert(user_input)
            break
        except (ValueError, ZeroDivisionError):
            pass
    display = gauge(decimal)
    print(display)

def convert(fraction):
    try:
        x, y = fraction.split("/")
        if not x.lstrip("-").isdigit() or not y.lstrip("-").isdigit():
            raise ValueError # X and Y must be integers.
            
        x = int(x)
        y = int(y)

        if y == 0:
            raise ZeroDivisionError # Y cannot be zero.

        if x < 0 or y < 0 or x > y:
            raise ValueError # X must be non-negative and less than or equal to Y and Y must be non-negative.
            
        return round((x / y) * 100)

    except (ValueError):
        raise ValueError("Invalid input")
    
    except ZeroDivisionError:
        raise ZeroDivisionError("Denominator cannot be zero")

def gauge(number):
    if number >= 99:
        return "F"
    elif number <= 1:
        return "E"
    else:
        return f"{number}%"



if __name__ == "__main__":
    main()