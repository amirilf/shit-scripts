import random
from os import system

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


def multipleBaseConvertor(number: int, bases: list[int], default=True) -> int:
    """
    - Gets the number and converts number to bases one by one according to the list

    - default:
        - True: we already know number is based on 10
        - False: number is based on the first item of bases

    - Two consecutive bases should not be the same
    """

    if default:
        last = 10
        for base in bases:
            number = convertBase(number, last, base)
            last = base
    else:
        last = bases[0]
        for base in bases[1::]:
            number = convertBase(number, last, base)
            last = base

    return number


def doMathematicalOperations(number: int, operations: list) -> int:
    """
    Takes a number and applies a set of simple mathematical operations to the number in order and returns the result.

    operations should be like => [ value , operator ]

    - e.g. number is 10
        - [ [10,'*'] ]          ==> will return 100
        - [ [12,'+'] ]          ==> will return 22
        - [ [5,'+'] , [3,'*'] ] ==> will return 45
    """

    for operation in operations:

        value = operation[0]
        operator = operation[1]

        match operator:
            case "+":
                number += value
            case "-":
                number -= value
            case "*":
                number *= value
            case "/":
                number /= value
            case _:
                raise Exception("Wrong operator detected, only (+,-,*,/) are accepted")

    return number


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
        - 1) range of seprators
        - 2) main seprator letter
        - 3) number of bases which we use in convertBase
        - 4) all the bases
        - 5) 3 numbers which we use for operations (+ and * and -)

        -> and there are lots of useless chars in between
    ]

    this key will be converted into parts with specified sizes and added to the beginning and end of the final message
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
    mainSepratorLetterIndex = random.randint(letterRange, 51 - letterRange)
    key += str(mainSepratorLetterIndex)

    # just a random letter in between and also it will be used to find the right ascii code of main letter
    key += randomLetter()

    # number of bases we use in converBase to make a magic number instead of a simple number
    bases = random.randint(5, 7)
    key += str(bases)

    # to check that there are no matching bases
    baseList = []

    for i in range(bases):

        # range of bases are in [5,14]
        randBase = random.randint(5, 14)

        # check the absence of duplicates
        while randBase in baseList or randBase == 10:
            randBase = random.randint(5, 14)
        baseList.append(randBase)

        key += str(randBase)

        # just a random letter to make it wierd
        if doIt():
            key += randomLetter()

    # these are values which we use to make numbers more strange
    # pattern for these 7 digits => [3 digit (+) | 1 digit (*) | 3 digit (-)]
    values = []

    values.append(random.randint(101, 299))
    values.append(random.randint(3, 9))
    values.append(random.randint(300, 499))

    for i in values:
        key += str(i)

        # just a random letter to make it wierd
        if doIt():
            key += randomLetter()

    keyLength = len(key)
    half = keyLength // 2
    firstPartOfKey = random.randint(half - 4, half + 4)
    secondPartOfKey = keyLength - firstPartOfKey

    return [
        key,
        firstPartOfKey,
        secondPartOfKey,
        baseList,
        mainSepratorLetterIndex,
        letterRange,
        values,
    ]


def createMessage(text: str) -> str:
    """
    create the final message
    get the key and then create the rest of it
    """

    # [key, firstPartOfKey, secondPartOfKey, baseList, mainSepratorLetter, letterRange, values]
    keyList = keyGenerator()

    result = ""

    for char in text:

        # get the ascii number of the letter
        charNumber = ord(char)

        # convertBase job
        charNumber = multipleBaseConvertor(charNumber, keyList[3])

        # math job
        charNumber = doMathematicalOperations(
            charNumber,
            [[keyList[6][0], "+"], [keyList[6][1], "*"], [keyList[6][2], "-"]],
        )

        # create rendom seprator char (which is in range)
        seprator = LETTERS[
            random.randint(keyList[4] - keyList[5], keyList[4] + keyList[5])
        ]

        # add the final result
        print(seprator)
        result += str(charNumber) + seprator

    print(keyList)
    print(result)


def readMessage():
    pass


system("cls || clear")
createMessage("b")
