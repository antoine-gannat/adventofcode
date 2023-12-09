import { read } from "../utils";

interface INode {
  name: string;
  left: INode;
  right: INode;
}

const fileContent = read(8).split("\n");

const instructions = fileContent[0].split("");

const nodes: INode[] = [];

fileContent.slice(2).forEach((line) => {
  const [_, current, left, right] = /(\w+) = \((\w+), (\w+)\)/gm.exec(line);
  let leftNode: INode = nodes.find((node) => node.name === left);

  if (!leftNode) {
    leftNode = { name: left, left: null, right: null };
    nodes.push(leftNode);
  }
  let rightNode: INode = nodes.find((node) => node.name === right);
  if (!rightNode) {
    rightNode = { name: right, left: null, right: null };
    nodes.push(rightNode);
  }
  let currentNode: INode = nodes.find((node) => node.name === current);
  if (!currentNode) {
    currentNode = { name: current, left: null, right: null };
    nodes.push(currentNode);
  }
  currentNode.left = leftNode;
  currentNode.right = rightNode;
});

let steps = 0;
let instructionIndex = 0;

let runningNodes: INode[] = nodes.filter((node) => node.name.endsWith("A"));
while (!runningNodes.every((n) => n.name.endsWith("Z"))) {
  steps++;
  const instruction = instructions[instructionIndex++];
  if (instructionIndex >= instructions.length) {
    instructionIndex = 0;
  }
  // run instruction on all running nodes
  runningNodes = runningNodes.map((node) =>
    instruction === "L" ? node.left : node.right
  );
}

console.log(steps);
