import { read } from "../utils";

interface INumbers {
  winningNumbers: number[];
  currentNumbers: number[];
}

const fileContent = read(4).split("\n");

const parsedNumberCache: Record<number, INumbers> = {};

function parseNumbers(cardId: number): INumbers {
  if (parsedNumberCache[cardId]) {
    return parsedNumberCache[cardId];
  }
  const line = fileContent[cardId];
  const startIndex = line.indexOf(":");
  // remove the beginning of the line
  const lineWithoutBeginning = line.substring(startIndex + 1);
  const [winningNumbersRaw, currentNumbersRaw] =
    lineWithoutBeginning.split("|");
  const regex = new RegExp(/(\d+)/gm);

  const winningNumbers = winningNumbersRaw
    .match(regex)
    .map((number) => parseInt(number, 10));
  const currentNumbers = currentNumbersRaw
    .match(regex)
    .map((number) => parseInt(number, 10));
  parsedNumberCache[cardId] = { winningNumbers, currentNumbers };
  return parsedNumberCache[cardId];
}

const cardsToCheck: number[] = fileContent.map((_, index) => index);
for (let i = 0; i < cardsToCheck.length; i++) {
  const cardId = cardsToCheck[i];
  const { winningNumbers, currentNumbers } = parseNumbers(cardId);

  const totalWinningNumbers = winningNumbers.reduce((acc, winningNumber) => {
    if (currentNumbers.includes(winningNumber)) {
      acc++;
    }
    return acc;
  }, 0);
  const newCopies = Array.from({ length: totalWinningNumbers })
    .map((_, index) => cardId + index + 1)
    .filter((id) => id < fileContent.length);
  cardsToCheck.push(...newCopies);
}

console.log(cardsToCheck.length);
