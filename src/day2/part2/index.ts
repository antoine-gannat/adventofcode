import { readInput } from "../../utils/readInput";

const data = readInput();

const parsedData = data.split("\r\n");

type CommandNames = "forward" | "up" | "down";

interface Position {
  x: number;
  y: number;
  aim: number;
}

interface Command {
  name: CommandNames;
  value: number;
}

let position: Position = {
  x: 0,
  y: 0,
  aim: 0,
};

function parseRawCommand(rawCommand: string): Command {
  const [name, value] = rawCommand.split(" ");
  return { name: name as CommandNames, value: Number(value) };
}

parsedData.forEach((rawCommand, index) => {
  const { name, value } = parseRawCommand(rawCommand);
  switch (name) {
    case "forward":
      position.x += value;
      position.y += position.aim * value;
      break;
    case "up":
      position.aim -= value;
      if (position.aim < 0) {
        position.aim = 0;
      }
      break;
    case "down":
      position.aim += value;
      break;
  }
});

console.log({ position });

console.log("result:", position.x * position.y);
