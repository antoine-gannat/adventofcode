import { readInput } from "../../utils/readInput";

const SLIDING_WINDOW_SIZE = 3;

const data = readInput();

const parsedData = data.split("\r\n").map(Number);

function getCumulOfNValues(
  startIndex: number,
  n: number = SLIDING_WINDOW_SIZE
): number {
  let total = 0;

  for (let i = 0; i < n; i++) {
    total += parsedData[startIndex + i];
  }
  return total;
}

let totalIncrease = 0;
// start at 1 since we can't compare from index 0
for (let i = 1; i < parsedData.length; i++) {
  const win1Total = getCumulOfNValues(i - 1);
  const win2Total = getCumulOfNValues(i);
  if (win2Total > win1Total) {
    totalIncrease++;
  }
}

console.log(totalIncrease);
