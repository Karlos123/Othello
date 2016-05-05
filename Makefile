# Makefile pre projekt z predmetu ICP
# Autori: Andrej Barna (xbarna01), Karel Jiranek (xjiran00)
# Ak. rok: 2015/2016


.PHONY: all clean pack

all: clean hracli hra

hracli:
	qmake -o src/Makefile src/hra2016-cli.pro
	make -C src/
#	make -C src/ clean

hra:
	qmake -o src/Makefile src/hra2016.pro
	make -C src/
#	make -C src/ clean

clean:
	-rm src/*.o src/moc_* src/qrc_* src/hra2016 src/hra2016-cli src/Makefile
#	-make -C src/ clean

pack:
	find ./src -not \( -name "moc_*" -or -name "qrc_*" \) -and \( -name "*.cpp" -or -name "*.hpp" \) | zip xbarna01-xjiran00.zip Makefile README.txt -@ src/res/* src/*.pro src/*qrc examples/ doc/
