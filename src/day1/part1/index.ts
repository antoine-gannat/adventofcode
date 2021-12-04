import { readInput } from "../../utils/readInput";

const data = readInput();

const parsedData = data.split("\r\n").map(Number);

let nbIncrease = 0;

parsedData.forEach((n, index) => {
  if (index > 0 && n > parsedData[index - 1]) {
    nbIncrease++;
  }
});

console.log(nbIncrease);
