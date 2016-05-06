# Makefile pre projekt z predmetu ICP
# Autori: Andrej Barna (xbarna01), Karel Jiranek (xjiran00)
# Ak. rok: 2015/2016


.PHONY: all clean pack run

all: hracli hra

hracli:
	qmake -o src/Makefile src/hra2016-cli.pro
	make -C src/
	mv src/hra2016-cli .

hra:
	qmake -o src/Makefile src/hra2016.pro
	make -C src/
	mv src/hra2016 .

clean:
	-make -C src/ clean || rm src/*.o src/moc_* src/qrc_*
	-rm hra2016 hra2016-cli src/Makefile

run: hracli hra
	./hra2016 &
	./hra2016-cli

doxygen:
	doxygen ./doc/config.con

pack:
	find ./src -not \( -name "moc_*" -or -name "qrc_*" \) -and \( -name "*.cpp" -or -name "*.hpp" \) | zip xbarna01-xjiran00.zip Makefile README.txt -@ src/res/* src/*.pro src/*qrc examples/* doc/config.con
