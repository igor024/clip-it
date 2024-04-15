EXEC = clipit

${EXEC}: main.cpp
	g++ $^ -o $@ #$^ is a variable for main.cpp, $@ is "clipit"


clean: 
	rm -f ${EXEC}