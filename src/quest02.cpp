#include <stdio.h>
#include <string.h>
#include "tools/readData.h"

typedef struct
{
    short length;
    const char *word;
} Word;

static char *quest02_1_data;
static char *quest02_2_data;

static const char *words_1[] = {
    "AL",
    "DO",
    "LL",
    "OL",
    "OR",
    "SI",
    "LOR",
    NULL};

static const Word words_2[] = {
    {10, "HCZCBQPOTD"},
    {10, "IHTUPFJPAF"},
    {10, "KORKMSNXKM"},
    {10, "KRWGKPLWWS"},
    {10, "LVPUKVXLEI"},
    {10, "MDNLHOQIQA"},
    {10, "QLRGDOFAEQ"},
    {10, "QWOGDEOMUG"},
    {10, "SZURECCYEH"},
    {10, "WYWVYMCXRL"},

    {4, "APSM"},
    {4, "CQZF"},
    {4, "CVTY"},
    {4, "CYZO"},
    {4, "DBJP"},
    {4, "EIQM"},
    {4, "LVHB"},
    {4, "MBOX"},
    {4, "PLVF"},
    {4, "ZBIM"},

    {3, "BIO"},
    {3, "GCI"},
    {3, "IFO"},
    {3, "KVD"},
    {3, "QVI"},
    {3, "SNM"},
    {3, "SRX"},
    {3, "TKJ"},
    {3, "WBN"},
    {3, "WOW"},
    {3, "ZNV"},

    {2, "AQ"},
    {2, "BI"},
    {2, "BV"},
    {2, "DG"},
    {2, "EQ"},
    {2, "FH"},
    {2, "OO"},
    {2, "OW"},
    {2, "SC"},
    {2, "UR"},
    {2, "WG"},
    {2, "YX"},

    {1, "A"},
    {1, "B"},
    {1, "D"},
    {1, "P"},
    {1, "S"},

    {0, NULL},
};

static const Word words_3[] = {
    {10, "ADAUEEZVFU"},
    {10, "DQBNEZVMGA"},
    {10, "EPOWEGNPUH"},
    {10, "FSKCODNWYE"},
    {10, "FVUODMKIZU"},
    {10, "HAJOPCTXJI"},
    {10, "HMUHBWALXQ"},
    {10, "IBUZVPBMJM"},
    {10, "MPTYSCWNUT"},
    {10, "QLCORJHVQQ"},
    {10, "QLUEECIPIN"},
    {10, "QTQBCDRFTP"},
    {10, "RDHAERHYKX"},
    {10, "TATUNIISAP"},
    {10, "TSWOFFKWIS"},
    {10, "UGGNNIYJAS"},
    {10, "WZPPHTMQKZ"},
    {10, "XGRCDOMRTC"},
    {10, "XHXBADYWOE"},
    {10, "YDXEQOKXQH"},

    {4, "BIFG"},
    {4, "CLDD"},
    {4, "CLRZ"},
    {4, "FAJU"},
    {4, "FIOD"},
    {4, "FTYX"},
    {4, "IBPF"},
    {4, "OPNI"},
    {4, "OYOW"},
    {4, "PAQN"},
    {4, "RDOT"},
    {4, "RNJW"},
    {4, "RYLC"},
    {4, "WMWF"},
    {4, "WUJL"},
    {4, "XLVI"},
    {4, "YJQK"},
    {4, "YORT"},
    {4, "ZBUZ"},
    {4, "ZXOH"},

    {3, "BAY"},
    {3, "CWA"},
    {3, "ESZ"},
    {3, "HCX"},
    {3, "HDO"},
    {3, "LOL"},
    {3, "LRC"},
    {3, "MRW"},
    {3, "MUG"},
    {3, "OIJ"},
    {3, "OKI"},
    {3, "QOT"},
    {3, "QPG"},
    {3, "RCN"},
    {3, "RIJ"},
    {3, "SND"},
    {3, "TRO"},
    {3, "WJJ"},
    {3, "XDT"},
    {3, "YSP"},
    {3, "ZTJ"},

    {2, "AO"},
    {2, "AX"},
    {2, "BC"},
    {2, "EP"},
    {2, "FG"},
    {2, "FZ"},
    {2, "GL"},
    {2, "GP"},
    {2, "GT"},
    {2, "HL"},
    {2, "HQ"},
    {2, "JY"},
    {2, "LM"},
    {2, "LO"},
    {2, "MV"},
    {2, "OO"},
    {2, "PF"},
    {2, "QK"},
    {2, "SS"},
    {2, "SY"},
    {2, "TP"},
    {2, "TZ"},

    {1, "C"},
    {1, "D"},
    {1, "I"},
    {1, "K"},
    {1, "M"},
    {1, "P"},
    {1, "Q"},
    {1, "U"},
    {1, "X"},
    {1, "Z"},
    {0, NULL},
};

static bool matches(const char *sentence, const char *word)
{
    while (*word)
    {
        if (*sentence < 32 || *word != *sentence)
        {
            return false;
        }
        word++;
        sentence++;
    }

    return true;
}

static bool reverseMatches(const char *sentence, const char *word, short len)
{
    for (short i = len; i > 0; i--, sentence++)
    {
        if (*sentence < 32 || word[i - 1] != *sentence)
        {
            return false;
        }
    }

    return true;
}

static unsigned int part1()
{
    unsigned short sum = 0;

    void *data = tools::readData("../data/quest02_1_data.raw");
    for (const char *sentence = (const char *)data; *sentence >= 32; sentence++)
    {
        for (int i = 0; words_1[i]; i++)
        {
            if (matches(sentence, words_1[i]))
            {
                sum++;
            }
        }
    }

    free(data);

    return sum;
}

static unsigned int part2()
{
    unsigned short sum = 0;
    short remaining = 0;

    void *data = tools::readData("../data/quest02_2_data.raw");
    const char *string = (const char *)data;
    int totalLength = strlen(string);

    const char *str = string;
    short index = 0;

    while (index < totalLength)
    {
        short length = tools::stringLength(str);

        for (const char *ptr = str; *ptr >= 32; ptr++)
        {
            for (short i = 0; words_2[i].length > 0; i++)
            {
                if (remaining >= words_2[i].length)
                {
                    break;
                }

                if (matches(ptr, words_2[i].word) || reverseMatches(ptr, words_2[i].word, words_2[i].length))
                {
                    remaining = words_2[i].length;
                    break;
                }
            }

            if (remaining > 0)
            {
                remaining--;
                sum++;
            }
        }
        str += length + 1;
        index += length + 1;
    }

    free(data);
    return sum;
}

class Data
{
private:
    const char *memory;
    const char *words;
    const char *data;

    char *buffer;
    int totalSize;

public:
    int width;
    int height;

public:
    Data(const char *memory)
    {
        this->memory = memory;
        if (!matches(memory, "WORDS:"))
        {
            throw;
        }
        this->words = memory + 6;
        while (*memory >= 32)
        {
            memory++;
        }
        while (*memory < 32)
        {
            memory++;
        }
        this->totalSize = strlen(memory);

        this->data = memory;
        this->width = tools::stringLength(memory);
        this->height = (this->totalSize + 1) / (width + 1);

        this->buffer = (char *)calloc(this->width * this->height, sizeof(char));
    }

    ~Data()
    {
        free(this->buffer);
        free((void *)this->memory);
    }

    const char get(int x, int y)
    {
        if (y < 0 || y >= this->height)
        {
            return 0;
        }

        x = (x + this->width) % this->width;

        int index = x + y * (this->width + 1);
        if (index >= this->totalSize)
        {
            throw;
        }
        const char c = this->data[index];
        if (c < 32)
        {
            throw;
        }
        return c;
    }

    void apply(int x, int y, const char *currentWord, int dx, int dy)
    {
        int size = this->width * this->height;

        for (int i = 0; currentWord[i] > ' ' && currentWord[i] != ','; i++)
        {
            int x1 = x + dx * i;
            int y1 = y + dy * i;
            int xx = (x1 + width) % width;
            int yy = y1;

            int index = xx + yy * this->width;
            if (index >= size)
            {
                throw;
            }
            this->buffer[index] = 1;
        }
    }

    void checkWords(int x, int y, int dx, int dy)
    {
        const char *currentWord = this->words;

        while (*currentWord >= 32)
        {
            bool ok = true;
            for (int i = 0; currentWord[i] > ' ' && currentWord[i] != ','; i++)
            {
                if (currentWord[i] != this->get(x + dx * i, y + dy * i))
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                this->apply(x, y, currentWord, dx, dy);
            }
            while (*currentWord > ' ' && *currentWord != ',')
            {
                currentWord++;
            }
            if (*currentWord == ',')
            {
                currentWord++;
            }
        }
    }

    int getTotal()
    {
        int total = 0;
        for (int i = 0; i < this->height * this->width; i++)
        {
            total += this->buffer[i];
        }
        return total;
    }
};

static unsigned int part3()
{
    void *data = tools::readData("../data/quest02_3_data.raw");

    Data string((const char *)data);

    for (int x = 0; x < string.width; x++)
    {
        for (int y = 0; y < string.height; y++)
        {
            string.checkWords(x, y, 1, 0);
            string.checkWords(x, y, -1, 0);
            string.checkWords(x, y, 0, 1);
            string.checkWords(x, y, 0, -1);
        }
    }

    return string.getTotal();
}

void quest02()
{
    printf("QUEST 2\n");

    unsigned int d1 = part1();
    printf("\tPART 1 = %u\n", d1);

    unsigned int d2 = part2();
    printf("\tPART 2 = %u\n", d2);

    unsigned int d3 = part3();
    printf("\tPART 3 = %u\n", d3);
}

int main(int count, char **args)
{
    quest02();
}
