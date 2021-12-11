import { readInputAsLines } from "../../utils/readInput";

type ClosingChars = ")" | "]" | "}" | ">";
type OpeningChars = "(" | "[" | "{" | "<";

interface ICharPair {
  opening: OpeningChars;
  closing: ClosingChars;
}

const charPairs: ICharPair[] = [
  {
    opening: "(",
    closing: ")",
  },
  {
    opening: "<",
    closing: ">",
  },
  {
    opening: "[",
    closing: "]",
  },
  {
    opening: "{",
    closing: "}",
  },
];

const data = readInputAsLines();

const pointsPerChar: { [key in ClosingChars]: number } = {
  ")": 3,
  "]": 57,
  "}": 1197,
  ">": 25137,
};

function getCharPair(char: string): ICharPair {
  return charPairs.find(
    ({ opening, closing }) => opening === char || closing === char
  );
}

function isOpeningChar(char: string): boolean {
  return char === "(" || char === "[" || char === "{" || char === "<";
}

function parseLine(line: string): true | ClosingChars {
  let openedStack: OpeningChars[] = [];
  for (let i = 0; i < line.length; i++) {
    const pair = getCharPair(line[i]);
    if (isOpeningChar(line[i])) {
      openedStack.push(pair.opening);
    } else {
      const removedEl = openedStack.pop();
      if (removedEl !== pair.opening) {
        return pair.closing;
      }
    }
  }
  return true;
}

let errorScore = 0;

data.forEach((line) => {
  const parsingResult = parseLine(line);
  if (parsingResult !== true) {
    errorScore += pointsPerChar[parsingResult];
  }
});

console.log("Result:", errorScore);
