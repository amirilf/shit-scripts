import random

# GLOBAL
LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


def convertBase(number: int, firstBase: int, secondBase: int) -> int:
    """
    This function gets a number based on `firstBase` &
    convert it to `secondBase`

    Only positive numbers are considered (:

    e.g.
        - 32 in base 4 to base 3 => 112
        - 1110 in base 2 to base 10 => 14
    """

    assert firstBase != secondBase, "Kidding? Bases are the same..."
    assert number >= 0, "Only a positive number!"
    assert firstBase >= 2 and secondBase >= 2, "Bases should be at least 2"

    # firstBase to base 10
    if firstBase != 10:

        power = 0
        tmp = number
        number = 0

        while tmp > 0:

            digit = tmp % 10
            tmp //= 10

            number += pow(firstBase, power) * digit
            power += 1

    # easy easy
    if secondBase == 10:
        return number

    # convert number (in base 10) to requested base
    result = ""

    while number > 0:
        result += str(number % secondBase)
        number //= secondBase

    return int(result[::-1])


def randomLetter() -> str:
    """
    returns a random english letter
    a-z and A-Z
    """
    return random.choice(LETTERS)


def doIt() -> bool:

    "just a random true or false"

    if random.randint(0, 1):
        return True
    return False


def keyGenerator() -> list:
    """
    This function creates a key for the message

    key pattern => [
        1 -> range of seprators
        2 -> main seprator letter
        3 -> number of bases which we use in convertBase
        4 -> all the bases
        5 -> 4 numbers which we use for operations (* and +)

        -> and there are lots of useless chars in between
    ]

    this key will be converted into two parts with specified sizes and added to the beginning and end of the final message
    parts are defined in `firstPartOfKey` and `secondPartOfKey`
    """

    key = randomLetter()

    # the range of accepted letters around seprator letter (now they are all seprators)
    letterRange = random.randint(1, 9)
    key += str(letterRange)

    # if it's (3,6,9) => 3 | (2,4,8) => 2 | (1,5,7) => 1
    # just a trick to make number between 1 to 9 and not between 1 to 3
    if letterRange % 3 == 0:
        letterRange = 3
    elif letterRange % 2 == 0:
        letterRange = 2
    else:
        letterRange = 1

    # add the main seprator letter
    # main seprator is the key letter which will seprate letters in complex result
    # and we can detect and seprate letters in last result by this letter and its neighbors (which are in range)
    mainSepratorLetter = ord(LETTERS[random.randint(letterRange, 51 - letterRange)])
    key += str(mainSepratorLetter)

    # just a random letter in between and also it will be used to find the right ascii code of main letter
    key += randomLetter()

    # number of bases we use in converBase to make a magic number instead of a simple number
    bases = random.randint(5, 7)
    key += str(bases)

    # to check that there are no matching bases
    lis = []

    for i in range(bases):

        # range of bases are in [5,14]
        randBase = random.randint(5, 14)

        # check the absence of duplicates
        while randBase in lis:
            randBase = random.randint(5, 14)
        lis.append(randBase)

        key += str(randBase)

        # just a random letter to make it wierd
        if doIt():
            key += randomLetter()

    # these are operators which we use to make numbers more strange
    # pattern for these 9 digits => [2 digit (*) | 3 digit (+) | 1 digit (*) | 3 digit (+)]
    key += str(random.randint(10, 19))

    # just a random letter to make it wierd
    if doIt():
        key += randomLetter()

    key += str(random.randint(201, 399))

    # just a random letter to make it wierd
    if doIt():
        key += randomLetter()

    key += str(random.randint(3, 6))

    # just a random letter to make it wierd
    if doIt():
        key += randomLetter()

    key += str(random.randint(151, 199))

    # just a random letter to make it wierd
    if doIt():
        key += randomLetter()

    keyLength = len(key)
    half = keyLength // 2
    firstPartOfKey = random.randint(half - 4, half + 4)
    secondPartOfKey = keyLength - firstPartOfKey

    return [firstPartOfKey, secondPartOfKey, key]


def createMessage():
    keyList = keyGenerator()
    print(keyList)


def readMessage():
    pass


createMessage()
