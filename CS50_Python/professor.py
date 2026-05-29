import random

# main funtion should loop and ask problems like int_list[0] + int_list[1], int_list[2] + int_list[3], etc. and keep track of the score.
def main():
    level = get_level()
    score = 0
    int_list = generate_integer(level)
    i = 0
    while i < len(int_list) - 1:
        x = int_list[i]
        y = int_list[i + 1]
        for _ in range(3): # user gets 3 chances to answer correctly
            try:
                answer = int(input(f"{x} + {y} = "))
                if answer == x + y:
                    score += 1
                    break
                else:
                    print("EEE")
            except ValueError:
                print("EEE")
                pass
        else:
            print(f"{x} + {y} = {x + y}")
        i = i + 2
    print(f"Score: {score}")


def get_level():
    while True:
        try:
            level = int(input("Level: "))
            if level in [1, 2, 3]:
                return level
        except ValueError:
            pass

# the generate integer function should return a list of 20 integers.

def generate_integer(level):
    int_list = []
    if level == 1:
        for _ in range(20):
            int_list.append(random.randint(0, 9))
        return int_list
    elif level == 2:
        for _ in range(20):
            int_list.append(random.randint(10, 99))
        return int_list
    elif level == 3:
        for _ in range(20):
            int_list.append(random.randint(100, 999))
        return int_list



if __name__ == "__main__":
    main()
