all: cw
main.o : main.c
	gcc -std=gnu99 -c main.c
changeTense.o : changeTense.c
	gcc -std=gnu99 -c changeTense.c
checkPresentText.o : checkPresentText.c
	gcc -std=gnu99 -c checkPresentText.c
cmp.o : cmp.c
	gcc -std=gnu99 -c cmp.c
commandError.o : commandError.c
	gcc -std=gnu99 -c commandError.c
courseWorkInfo.o : courseWorkInfo.c
	gcc -std=gnu99 -c courseWorkInfo.c
deleteDuplicates.o : deleteDuplicates.c
	gcc -std=gnu99 -c deleteDuplicates.c
deleteLongSentences.o : deleteLongSentences.c
	gcc -std=gnu99 -c deleteLongSentences.c
findWordInOtherSentences.o : findWordInOtherSentences.c
	gcc -std=gnu99 -c findWordInOtherSentences.c
getWords.o : getWords.c
	gcc -std=gnu99 -c getWords.c
help.o : help.c
	gcc -std=gnu99 -c help.c
structures.o : structures.c
	gcc -std=gnu99 -c structures.c
input_text.o : input_text.c
	gcc -std=gnu99 -c input_text.c
output.o : output.c
	gcc -std=gnu99 -c output.c
selectionWithText.o : selectionWithText.c
	gcc -std=gnu99 -c selectionWithText.c
splitSentence.o : splitSentence.c
	gcc -std=gnu99 -c splitSentence.c
splitText.o : splitText.c
	gcc -std=gnu99 -c splitText.c
wordsToUpper.o : wordsToUpper.c
	gcc -std=gnu99 -c wordsToUpper.c
cw : main.o changeTense.o checkPresentText.o cmp.o commandError.o courseWorkInfo.o deleteDuplicates.o deleteLongSentences.o findWordInOtherSentences.o getWords.o help.o structures.o input_text.o output.o selectionWithText.o splitSentence.o splitText.o wordsToUpper.o
	gcc main.o changeTense.o checkPresentText.o cmp.o commandError.o courseWorkInfo.o deleteDuplicates.o deleteLongSentences.o findWordInOtherSentences.o getWords.o help.o structures.o input_text.o output.o selectionWithText.o splitSentence.o splitText.o wordsToUpper.o -o cw
	rm *.o