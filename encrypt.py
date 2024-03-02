import random
from os import system

# GLOBAL
LETTERS = "iHpqrdeOMNVWyoAbRghZzuSTcGDBkEaCjfvwYJPQUlmnIxLKXstF"


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


def sameRemainderLetters(remainder: int, size: int) -> list[str]:
    """
    Returns all letters with same index remainder to size

    e.g. if remainder is 1 and size is 5
        ->  indexes => (1,6,11,16,21,26,31,...,51)
        -> and all the letters these indexes will be returned from LETTERS
    """

    return LETTERS[remainder::size]


def doIt() -> bool:

    "just a random true or false"

    if random.randint(0, 1):
        return True
    return False


def keyGenerator() -> list:
    """
    - Generate a key for the message

    - key pattern:
        - 1) range of seprators
        - 2) main seprator letter
        - 3) number of bases which we use in convertBase
        - 4) all the bases
        - 5) 3 numbers which we use for operations (+ and * and -)

        -> and there are lots of useless chars in between

    This key will be converted into parts with specified sizes and added to the beginning and end of the final message
    parts are defined in `firstPartOfKey` and `secondPartOfKey`
    """

    # random char
    key = randomLetter()

    # define acceptable letters to be seprators in final message
    divisorSize = random.randint(6, 9)
    sepratorRemainder = random.randint(0, divisorSize - 1)
    key += str(sepratorRemainder + (divisorSize * random.randint(11, 99)))

    # add the letter with index of divisorSize to decode it
    key += LETTERS[divisorSize]

    # random char
    key += randomLetter()

    # number of bases we use in converBase to make a magic number instead of a simple number
    bases = random.randint(5, 7)
    key += str(bases)

    # to check that there are no same bases
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
    operationValues = []

    operationValues.append(random.randint(101, 299))
    operationValues.append(random.randint(3, 9))
    operationValues.append(random.randint(300, 499))

    for i in operationValues:
        key += str(i)

        # just a random letter to make it wierd
        if doIt():
            key += randomLetter()

    keyLength = len(key)
    half = keyLength // 2
    firstPartOfKey = random.randint(half - 5, half + 5)
    secondPartOfKey = keyLength - firstPartOfKey

    return [
        key,
        baseList,
        operationValues,
        divisorSize,
        sepratorRemainder,
        [firstPartOfKey, secondPartOfKey],
    ]


def createMessage(text: str) -> str:
    """
    create the final message
    get the key and then create the rest of it
    """

    keyList = keyGenerator()

    # define variables
    key = keyList[0]
    baseList = keyList[1]
    operationValues = keyList[2]
    divisorSize = keyList[3]
    sepratorRemainder = keyList[4]
    keyParts = keyList[5]
    seprators = sameRemainderLetters(sepratorRemainder, divisorSize)
    body = ""
    result = ""

    for char in text:

        # get the ascii number of the letter
        charNumber = ord(char)

        # convertBase job
        charNumber = multipleBaseConvertor(charNumber, baseList)

        # math job
        charNumber = doMathematicalOperations(
            charNumber,
            list(zip(operationValues, ["+", "*", "-"])),
        )

        body += str(charNumber) + random.choice(seprators)

    firstPartKey = key[: keyParts[0]]
    secondPartKey = key[keyParts[0] :]

    """
    In the last step, we have to insert the first and last size of the key length.
    First, we specify a random base for each, and then we change the rest number from the specified base to base 10, and finally
    We get the first and last length that we have to cut to collect the key parts and make the key.
    
    trickBase => the random base
    and we can find the number bcs it's surronded by the trickBase and a random letter
    
    keyPart[0] and [1] are length of key parts that we finally wanna insert them in key with the explained way
    """

    # beginning
    trickBase = random.randint(6, 9)
    result = str(trickBase)
    result += str(convertBase(keyParts[0], 10, trickBase) * trickBase)
    result += randomLetter()

    # main
    result += firstPartKey
    result += body
    result += secondPartKey

    # end
    result += randomLetter()
    trickBase = random.randint(6, 9)
    result += str(convertBase(keyParts[1], 10, trickBase) * trickBase)
    result += str(trickBase)

    print(firstPartKey)
    print(secondPartKey)
    print(keyList)
    print(body)
    print(result)


def readMessage():
    pass


system("cls || clear")
createMessage("abcde")
