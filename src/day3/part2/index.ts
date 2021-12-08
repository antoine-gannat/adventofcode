import { readInput } from "../../utils/readInput";

interface IBitIndexCount {
  zeroCount: number;
  oneCount: number;
}

type FilterFct = (
  numbers: string[],
  count: IBitIndexCount,
  bitIndex: number
) => string[];

const data = readInput();

const binaryNumbers = data.split("\r\n");

const numbersLength = binaryNumbers[0].length;

/**
 * Function that make a sum of each bit of numbers and call a filter function to have them filtered.
 */
function filterNumbers(numbers: string[], filterFct: FilterFct): string[] {
  for (
    let bitIndex = 0;
    bitIndex < numbersLength && numbers.length > 1;
    bitIndex++
  ) {
    const indexCount: IBitIndexCount = { zeroCount: 0, oneCount: 0 };
    // Count bits based on their position
    for (let i = 0; i < numbers.length; i++) {
      if (numbers[i][bitIndex] === "1") {
        indexCount.oneCount++;
      } else {
        indexCount.zeroCount++;
      }
    }
    numbers = filterFct(numbers, indexCount, bitIndex);
  }
  return numbers;
}

const oxygenNumbers = filterNumbers(
  [...binaryNumbers], // pass a copy of the input
  (numbers, count, bitIndex) => {
    // if there is more zeros than ones at that bit index
    if (count.zeroCount > count.oneCount) {
      // only keep oxygen numbers which have a zero at that position
      return numbers.filter((n) => n[bitIndex] === "0");
    } else {
      // only keep oxygen numbers which have a one at that position
      return numbers.filter((n) => n[bitIndex] === "1");
    }
  }
);

const scrubberNumbers = filterNumbers(
  [...binaryNumbers], // pass a copy of the input
  (numbers, count, bitIndex) => {
    // if there is more zeros than ones at that bit index
    if (count.zeroCount > count.oneCount) {
      // only keep oxygen numbers which have a zero at that position
      return numbers.filter((n) => n[bitIndex] === "1");
    } else {
      // only keep oxygen numbers which have a one at that position
      return numbers.filter((n) => n[bitIndex] === "0");
    }
  }
);

const oxygenGeneratorRating = parseInt(oxygenNumbers[0], 2);
const co2ScrubberRating = parseInt(scrubberNumbers[0], 2);

console.log("result:", oxygenGeneratorRating * co2ScrubberRating);
