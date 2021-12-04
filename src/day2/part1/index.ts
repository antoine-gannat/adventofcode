import { readInput } from "../../utils/readInput";

const data = readInput();

const parsedData = data.split("\r\n");

type CommandNames = "forward" | "up" | "down";

interface Position {
  x: number;
  y: number;
}

interface Command {
  name: CommandNames;
  value: number;
}

let position: Position = {
  x: 0,
  y: 0,
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
      break;
    case "up":
      position.y -= value;
      if (position.y < 0) {
        position.y = 0;
      }
      break;
    case "down":
      position.y += value;
      break;
  }
});

console.log({ position });

console.log("result:", position.x * position.y);
