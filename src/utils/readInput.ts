import * as fs from "fs";
import { argv } from "process";

export function readInput(): string {
  if (argv.length < 2) {
    throw Error("Missing input file");
  }

  const data = fs.readFileSync(argv[2], {
    encoding: "utf8",
  });
  return data;
}

export function readInputAsLines(): string[] {
  const rawData = readInput();

  return rawData.split("\r\n");
}
