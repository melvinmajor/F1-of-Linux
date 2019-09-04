SRC_DIR=../src
SRC=$(wildcard $(SRC_DIR)/*.c)
SRC+=$(wildcard $(SRC_DIR)/*.h)
SRC_SORTED=$(sort $(SRC))

REPORT_IN=report.md
REPORT_OUT=report.pdf
CODE_MD=code.md

.PHONY: compile run format watch clean report

eisvogel.tex:
	wget -O $@ 'https://raw.githubusercontent.com/Wandmalfarbe/pandoc-latex-template/master/eisvogel.tex' 

report: $(REPORT_OUT)

$(REPORT_OUT): $(REPORT_IN) eisvogel.tex $(CODE_MD)
	pandoc -f markdown --template eisvogel.tex \
	    -o $@ --listings --pdf-engine xelatex \
	    $(REPORT_IN) $(CODE_MD)

$(CODE_MD): $(SRC_SORTED)
	./gencode-md.sh $(SRC_SORTED) > $(CODE_MD)
