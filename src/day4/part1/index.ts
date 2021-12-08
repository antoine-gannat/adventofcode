import { readInput } from "../../utils/readInput";

// numbers are stored in a 2d array
type BingoBoard = Array<Array<number>>;

// winning numbers in board are set as the following
const CROSSED_OUT_NUMBER = -1;
const BOARD_SIZE = 5;

function parseBoards(rawLines: string[]): BingoBoard[] {
  const result: BingoBoard[] = [];
  let newBoard: BingoBoard = [];
  rawLines.forEach((line, index) => {
    // if there is a new line, we need to create a new bingo board
    if (line.length === 0) {
      // add the board to the result list
      newBoard.length > 0 && result.push(newBoard);
      // reset the board
      newBoard = [];
      return;
    }
    // add a new bingo line
    newBoard.push(
      line
        .split(" ")
        .filter((n) => n.trim().length > 0)
        .map((n) => Number(n))
    );
  });
  return result;
}

function crossOutNumber(board: BingoBoard, number: number): BingoBoard {
  for (let line = 0; line < board.length; line++) {
    for (let col = 0; col < board[line].length; col++) {
      if (board[line][col] === number) {
        board[line][col] = CROSSED_OUT_NUMBER;
      }
    }
  }
  return board;
}

function checkBoardWin(board: BingoBoard): boolean {
  for (let index = 0; index < BOARD_SIZE; index++) {
    // check all numbers on that line
    let isLineWin = true;
    for (let col = 0; col < BOARD_SIZE; col++) {
      if (board[index][col] !== CROSSED_OUT_NUMBER) {
        isLineWin = false;
        break;
      }
    }
    if (isLineWin) {
      return true;
    }
    let isColWin = true;
    // check all numbers on that column
    for (let line = 0; line < BOARD_SIZE; line++) {
      if (board[line][index] !== CROSSED_OUT_NUMBER) {
        isColWin = false;
        break;
      }
    }
    if (isColWin) {
      return true;
    }
  }
  return false;
}

function sumBoardNonWinningNumber(board: BingoBoard): number {
  let total = 0;
  for (let line = 0; line < board.length; line++) {
    for (let col = 0; col < board[line].length; col++) {
      const number = board[line][col];
      if (number !== CROSSED_OUT_NUMBER) {
        total += number;
      }
    }
  }
  return total;
}

// input parsing
const data = readInput();
const lines = data.split("\r\n"); //remove the first line and split in lines
const winningNumbers = lines[0].split(",").map(Number); // first line contains the winning numbers

let boards = parseBoards(lines.slice(1));

// for each winning number
for (const winNumber of winningNumbers) {
  // cross out winning numbers in each board
  boards = boards.map((board) => crossOutNumber(board, winNumber));
  // then check for a win
  const winningBoards = boards.filter(checkBoardWin);
  if (winningBoards.length > 0) {
    console.log(
      "Result: ",
      sumBoardNonWinningNumber(winningBoards[0]) * winNumber
    );
    break;
  }
}
