import * as fs from "fs";
import path = require("path");

export function read(day: number): string {
  return fs
    .readFileSync(path.join(__dirname, "..", "inputs", `day${day}.txt`), "utf8")
    .toLowerCase();
}
