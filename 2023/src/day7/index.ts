import { read } from "../utils";

interface IHand {
  cards: string[];
  bet: number;
}

const cards = ["A", "K", "Q", "T", "9", "8", "7", "6", "5", "4", "3", "2", "J"];

function howMuchOfAKind(cards: string[]): [number, number] {
  const counts = cards.reduce((acc, card) => {
    acc[card] = acc[card] ? acc[card] + 1 : 1;
    return acc;
  }, {} as { [key: string]: number });
  const numberOfJokers = counts["J"] ?? 0;
  counts["J"] = 0; // don't count jokers for the following
  const sortedCount = Object.values(counts).sort();
  return [
    sortedCount[sortedCount.length - 1] + numberOfJokers,
    sortedCount[sortedCount.length - 2],
  ];
}

function compareHand(hand1: string[], hand2: string[]): number {
  const hand1MaxOfAKind = howMuchOfAKind(hand1);
  const hand2MaxOfAKind = howMuchOfAKind(hand2);

  if (hand1MaxOfAKind[0] > hand2MaxOfAKind[0]) {
    return 1;
  }
  if (hand1MaxOfAKind[0] < hand2MaxOfAKind[0]) {
    return -1;
  }
  // check if there is a second pair
  if (hand1MaxOfAKind[1] > hand2MaxOfAKind[1]) {
    return 1;
  }
  if (hand1MaxOfAKind[1] < hand2MaxOfAKind[1]) {
    return -1;
  }
  // check the rest of the cards
  for (let i = 0; i < hand1.length; i++) {
    if (cards.indexOf(hand1[i]) < cards.indexOf(hand2[i])) {
      return 1;
    }
    if (cards.indexOf(hand1[i]) > cards.indexOf(hand2[i])) {
      return -1;
    }
  }
  return 0;
}

const fileContent = read(7).split("\n");

const hands = fileContent
  .map<IHand>((line) => {
    const [cards, bet] = line.split(" ");

    return {
      cards: cards.toUpperCase().split(""),
      bet: Number(bet),
    };
  })
  .sort((hand1, hand2) => {
    const result = compareHand(hand1.cards, hand2.cards);
    return result;
  });

console.log(
  hands.reduce((acc, hand, index) => acc + (index + 1) * hand.bet, 0)
);
