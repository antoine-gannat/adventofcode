import { read } from "../utils";

const exceptionMap: Record<string, [string, string]> = {
  oneight: ["one", "eight"],
  twone: ["two", "one"],
  eightwo: ["eight", "two"],
  sevenine: ["seven", "nine"],
  eighthree: ["eight", "three"],
  fiveight: ["five", "eight"],
  nineight: ["nine", "eight"],
};

const stringToNumberMap: Record<string, string> = {
  one: "1",
  two: "2",
  three: "3",
  four: "4",
  five: "5",
  six: "6",
  seven: "7",
  eight: "8",
  nine: "9",
};

function findNumbersInLine(line: string): string[] {
  const regex = new RegExp(
    /\d|oneight|twone|eightwo|sevenine|eighthree|fiveight|nineight|one|two|three|four|five|six|seven|eight|nine/gim
  );
  const numbers: string[] = [];

  let res;
  while ((res = regex.exec(line)) !== null) {
    const exception = exceptionMap[res[0]];
    if (exception) {
      numbers.push(exception[0], exception[1]);
    } else {
      numbers.push(res[0]);
    }
  }
  return numbers.map((number) => stringToNumberMap[number] ?? number);
}

const fileContent = read(1);

const lines = fileContent.split("\n");

const numbers: number[] = [];

lines.forEach((line) => {
  const numbersInLine = findNumbersInLine(line);
  const firstDigit = numbersInLine[0];
  const lastDigit = numbersInLine[numbersInLine.length - 1];
  numbers.push(Number(firstDigit + lastDigit));
});

console.log(
  "Total:",
  numbers.reduce((acc, curr) => acc + curr, 0)
);
