import { readInputAsLines } from "../../utils/readInput";

type Segment = "a" | "b" | "c" | "d" | "e" | "f" | "g";
type SegmentPosition =
  | "topLeft"
  | "top"
  | "topRight"
  | "center"
  | "bottomRight"
  | "bottom"
  | "bottomLeft";

interface INumber {
  segments: Segment[];
  value: number | null;
}

const segmentsPositions: { [position in SegmentPosition]: Segment | null } = {
  top: null,
  topLeft: null,
  topRight: null,
  center: null,
  bottom: null,
  bottomLeft: null,
  bottomRight: null,
};

/**
 * Find simple segments based on their length.
 */
function findSimpleNumbers(segments: Segment[]): number | null {
  switch (segments.length) {
    case 2:
      return 1;
    case 3:
      return 7;
    case 4:
      return 4;
    case 7:
      return 8;
  }
  return null;
}

function findNumber6(numbers: INumber[]) {
  // By finding the right segments, we can isolate number 6
  // The right segments are just number 1 segments
  const rightSegments = numbers.find((n) => n.value === 1).segments;
  // Only 3 numbers have 6 segments: 2, 5 and 6
  // Out of these three, only number 6 doesn't have both right segments
  const index = numbers.findIndex(
    (n) =>
      n.segments.length === 6 &&
      n.segments.includes(rightSegments[0]) !==
        n.segments.includes(rightSegments[1])
  );
  numbers[index].value = 6;
}

function findNumber2And3And5(numbers: INumber[]) {
  // number 2, 3 and 5 all have 5 segments

  // And we know both right segments and their position
  // so by using them we can know which is which

  // numbers 2 has topRight but not bottomRight
  numbers.find(
    ({ segments }) =>
      segments.length === 5 &&
      segments.includes(segmentsPositions["topRight"]) &&
      !segments.includes(segmentsPositions["bottomRight"])
  ).value = 2;

  // number 3 has both
  numbers.find(
    ({ segments }) =>
      segments.length === 5 &&
      segments.includes(segmentsPositions["topRight"]) &&
      segments.includes(segmentsPositions["bottomRight"])
  ).value = 3;

  // number 5 is the remaining one
  numbers.find(
    ({ segments, value }) => segments.length === 5 && value === null
  ).value = 5;
}

function findNumbers9And0(numbers: INumber[]) {
  // number zero can be found because it doesn't have the center segment

  numbers.filter(
    (n) =>
      n.segments.length === 6 &&
      !n.segments.includes(segmentsPositions["center"])
  )[0].value = 0;
  numbers.filter(
    (n) =>
      n.segments.length === 6 &&
      n.segments.includes(segmentsPositions["topRight"]) &&
      n.segments.includes(segmentsPositions["center"])
  )[0].value = 9;
}

function findRemainingSegments(numbers: INumber[]) {
  // center and bottom segments are the last one to be found
  // we can find the center one by looking at number 4 last unknown segment

  const num4 = numbers.find((n) => n.value === 4);
  segmentsPositions["center"] = num4.segments.filter(
    (s) =>
      s !== segmentsPositions["topLeft"] &&
      s !== segmentsPositions["topRight"] &&
      s !== segmentsPositions["bottomRight"]
  )[0];

  // now we can find the remaining segment which is not assigned to any position
  const possibleSegments: Segment[] = ["a", "b", "c", "d", "e", "f", "g"];

  segmentsPositions["bottom"] = possibleSegments.filter(
    (s) =>
      // for each known segment position
      Object.keys(segmentsPositions).filter(
        (key) => segmentsPositions[key as SegmentPosition] === s
      ).length === 0
  )[0];
}

function findLeftSegments(numbers: INumber[]) {
  const num2 = numbers.find((n) => n.value === 2);
  const num3 = numbers.find((n) => n.value === 3);
  const num5 = numbers.find((n) => n.value === 5);
  // the top left segment is present on 5 but not 3

  segmentsPositions["topLeft"] = num5.segments.filter(
    (s) => !num3.segments.includes(s)
  )[0];

  // the bottom left segment is only present on number 2 and not 3

  segmentsPositions["bottomLeft"] = num2.segments.filter(
    (s) => !num3.segments.includes(s)
  )[0];
}

function findTopSegment(numbers: INumber[]) {
  // Top segment can be found by finding the only segment not used by 1 but used by 7
  const num1 = numbers.find((n) => n.value === 1);
  const num7 = numbers.find((n) => n.value === 7);

  // find the segment in number 7 which is not in number 1
  segmentsPositions["top"] = num7.segments.filter(
    (s) => !num1.segments.includes(s)
  )[0];
}

function findRightSegments(numbers: INumber[]) {
  // we already know which are the two right segments, just not which is which
  // To find that we can look at which segment is in 6, it will be the bottom one

  const num6 = numbers.find((n) => n.value === 6);
  const num1 = numbers.find((n) => n.value === 1);

  const rightSegments = num1.segments;

  segmentsPositions["bottomRight"] = rightSegments.filter((s) =>
    num6.segments.includes(s)
  )[0];
  segmentsPositions["topRight"] = rightSegments.filter(
    (v) => v !== segmentsPositions["bottomRight"]
  )[0];
}

const data = readInputAsLines().map((line) => {
  const rawSplitedLine = line.split("|");

  return {
    uniqueSignals: rawSplitedLine[0]
      .split(" ")
      .map((signal) => signal.trim())
      .filter((s) => s.length > 0),
    outputValues: rawSplitedLine[1]
      .split(" ")
      .map((value) => value.trim())
      .filter((s) => s.length > 0),
  };
});

function decodeOutputNumber(outputNum: string, numbers: INumber[]): string {
  const outputNumberSegments = outputNum.split("");
  // find the number which has all segments from the output number
  const number = numbers.filter(
    ({ segments }) =>
      segments.length === outputNumberSegments.length &&
      outputNumberSegments.every((seg) => segments.includes(seg as Segment))
  )[0];
  return String(number.value);
}

let finalResult = 0;

data.forEach(({ uniqueSignals, outputValues }) => {
  const numbers: INumber[] = [];
  // load the segments from unique signals
  uniqueSignals.forEach((signal) => {
    const segments = signal.split("") as Segment[];
    numbers.push({
      segments,
      // find simple values (1, 4 and 7)
      value: findSimpleNumbers(segments),
    });
  });
  // find the top segment
  findTopSegment(numbers);
  // find 6
  findNumber6(numbers);
  findRightSegments(numbers);
  findNumber2And3And5(numbers);
  findLeftSegments(numbers);
  findRemainingSegments(numbers);
  findNumbers9And0(numbers);
  // now we have all numbers and segments position so we can start decoding

  const decodedNumbers = outputValues.map((outputVal) =>
    decodeOutputNumber(outputVal, numbers)
  );
  // build the full numbers
  finalResult += Number(decodedNumbers.join(""));
});
console.log({ finalResult });
