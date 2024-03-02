import random
from os import system
from string import ascii_letters, digits

# GLOBAL
LETTERS = "iHpqrdeOMNVWyoAbRghZzuSTcGDBkEaCjfvwYJPQUlmnIxLKXstF"
BLABLA = ["(", "]", ".", ">", ",", "#", "!", ":", "@", "$", "&", "*", "^"]


def isLetter(char: str) -> bool:
    """
    Assume we only have ascii letters and digits
    this function returns True if `char` is a letter
    and False if it's a digit
    """

    if 48 <= ord(char) and ord(char) <= 57:
        return False
    return True


def convertBase(number: int, firstBase: int, secondBase: int) -> int:
    """
    This function gets a number based on `firstBase` &
    convert it to `secondBase`

    Only positive numbers are considered (:
    All the bases should be <= 10

    e.g.
        - 32 in base 4 to base 3 => 112
        - 1110 in base 2 to base 10 => 14
    """

    assert firstBase != secondBase, "Kidding? Bases are the same..."
    assert number >= 0, "Only a positive number!"
    assert firstBase >= 2 and secondBase >= 2, "Bases should be at least 2"
    assert firstBase <= 10 and secondBase <= 10, "Bases should be at most 10"

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
                number //= value
            case _:
                raise Exception("Wrong operator detected, only (+,-,*,/) are accepted")

    return number


def randomLetter() -> str:
    """
    returns a random english letter
    a-z and A-Z
    """
    return random.choice(LETTERS)


def sameRemainderLetters(remainder: int, size: int) -> str:
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
    bases = random.randint(3, 5)
    key += str(bases)

    # to check that there are no same bases
    baseList = []

    for i in range(bases):

        # range of bases are in [5,9]
        randBase = random.randint(5, 9)

        # check the absence of duplicates
        while randBase in baseList:
            randBase = random.randint(5, 9)
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
    firstLength = random.randint(half - 5, half + 5)
    secondLength = keyLength - firstLength

    return [
        key,
        baseList,
        operationValues,
        divisorSize,
        sepratorRemainder,
        firstLength,
        secondLength,
    ]


def addRandomCharsInBody(text: str, limitedLetterList: str) -> str:

    result = ""
    choiceList = "".join([i for i in LETTERS if i not in limitedLetterList])

    for char in text:
        result += char

        if random.randint(1, 5) == 1:
            result += random.choice(choiceList)

    return result


def removeRandomCharsInBody(text: str, limitedLetterList: str) -> str:

    result = ""
    choiceList = "".join([i for i in LETTERS if i not in limitedLetterList])

    for char in text:
        if char not in choiceList:
            result += char

    return result


def addShitChars(text: str) -> str:
    """
    Make message more dirtier
    """

    firstIndex = random.randint(5, 10)
    secondIndex = random.randint(20, 25)

    insertText1 = (
        random.choice(BLABLA)
        + "".join(random.choices((ascii_letters + digits), k=random.randint(4, 8)))
        + random.choice(BLABLA)
    )
    insertText2 = (
        random.choice(BLABLA)
        + "".join(random.choices((ascii_letters + digits), k=random.randint(4, 8)))
        + random.choice(BLABLA)
    )

    text = (
        text[:firstIndex]
        + insertText1
        + text[firstIndex:secondIndex]
        + insertText2
        + text[secondIndex:]
    )

    return text


def removeShitChars(text: str) -> str:
    indexes = []
    for index in range(len(text)):
        if text[index] in BLABLA:
            indexes.append(index)
    return (
        text[: indexes[0]] + text[indexes[1] + 1 : indexes[2]] + text[indexes[3] + 1 :]
    )


def keyBodySeprator(text: str) -> list:

    fBase = int(text[0])
    fLength = ""
    for i in text[1:]:
        if not isLetter(i):  # so it's a digit
            fLength += i
        else:
            break
    beginIndex = 2 + len(fLength)
    fLength = int(fLength)

    sBase = int(text[-1])
    sLength = ""
    for i in text[-2::-1]:
        if not isLetter(i):  # so it's a digit
            sLength += i
        else:
            break
    endIndex = 2 + len(sLength)
    sLength = int(sLength[::-1])

    text = text[beginIndex:-endIndex:]

    fLength //= fBase
    sLength //= sBase

    fLength = convertBase(fLength, fBase, 10)
    sLength = convertBase(sLength, sBase, 10)

    key = text[:fLength] + text[-sLength:]
    msg = text[fLength:-sLength]

    return [key, msg]


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
    firstLength = keyList[5]
    secondLength = keyList[6]

    seprators = sameRemainderLetters(sepratorRemainder, divisorSize)

    body = ""
    result = ""

    # Splitting the key into two parts (with random sizes not necessarily same sizes )
    firstPartKey = key[:firstLength]
    secondPartKey = key[firstLength:]

    index = -1
    bases = len(baseList)

    for char in text:

        index += 1

        # get the ascii number of the letter
        charNumber = ord(char)

        # convert bases
        charNumber = convertBase(charNumber, 10, baseList[index % bases])

        # math operations
        charNumber = doMathematicalOperations(
            charNumber,
            list(zip(operationValues, ["+", "*", "-"])),
        )

        body += str(charNumber) + random.choice(seprators)

    body = addRandomCharsInBody(body, seprators)

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
    result += str(convertBase(firstLength, 10, trickBase) * trickBase)
    result += randomLetter()

    # main
    result += firstPartKey
    result += body
    result += secondPartKey

    # end
    result += randomLetter()
    trickBase = random.randint(6, 9)
    result += str(convertBase(secondLength, 10, trickBase) * trickBase)
    result += str(trickBase)

    # add some extra chars
    result = addShitChars(result)

    # final message is ready (:
    return result


def readMessage(message: str) -> str:
    message = removeShitChars(message)
    key, body = keyBodySeprator(message)

    sepratorRemainder = ""
    index = 1
    while True:
        char = key[index]
        if not isLetter(char):
            sepratorRemainder += char
            index += 1
        else:
            break
    divisorSize = LETTERS.index(key[index])
    sepratorRemainder = int(sepratorRemainder) % divisorSize

    seprators = sameRemainderLetters(sepratorRemainder, divisorSize)
    body = removeRandomCharsInBody(body, seprators)

    index += 2
    bases = int(key[index])
    index += 1

    baseList = []
    checker = 0

    while checker < bases:
        try:
            char = int(key[index])
            if char == 1:
                index += 1
                baseList.append(10 + int(key[index]))
            else:
                baseList.append(char)
            checker += 1
        except:
            pass
        finally:
            index += 1

    operationValues = []

    values = "".join([i for i in key[index:] if not isLetter(i)])
    operationValues.append(int(values[:3]))
    operationValues.append(int(values[3]))
    operationValues.append(int(values[4:]))

    # ===End of key decryption===

    result = ""
    letter = ""
    index = -1

    for char in body:
        if isLetter(char):
            index += 1
            letterNumber = int(letter)
            letter = ""

            letterNumber = doMathematicalOperations(
                letterNumber, list(zip(operationValues[::-1], ["+", "/", "-"]))
            )

            letterNumber = convertBase(letterNumber, baseList[index % bases], 10)

            result += chr(letterNumber)

        else:
            letter += char

    return result


# ==============
system("cls || clear")
msg = "hello guys!"
print("message:", msg)
msg = createMessage(msg)
print("encrypted:", msg)
msg = readMessage(msg)
print("message:", msg)
