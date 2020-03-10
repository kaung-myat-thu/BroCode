// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo(struct _frozen *destination) {
    struct frozen_desc frozen_modules[] = {
        {"base64", 3792036, 11253},
        {"codecs", 2809561, 36628},
        {"copy_reg", 4375358, 5137},
        {"encodings", 5425761, -4362},
        {"encodings.aliases", 5430123, 8760},
        {"encodings.ascii", 5438883, 2253},
        {"encodings.base64_codec", 5441136, 3829},
        {"encodings.big5", 5444965, 1748},
        {"encodings.big5hkscs", 5446713, 1788},
        {"encodings.bz2_codec", 5448501, 4721},
        {"encodings.charmap", 5453222, 3465},
        {"encodings.cp037", 5456687, 2829},
        {"encodings.cp1006", 5459516, 2915},
        {"encodings.cp1026", 5462431, 2843},
        {"encodings.cp1140", 5465274, 2829},
        {"encodings.cp1250", 5468103, 2866},
        {"encodings.cp1251", 5470969, 2863},
        {"encodings.cp1252", 5473832, 2866},
        {"encodings.cp1253", 5476698, 2879},
        {"encodings.cp1254", 5479577, 2868},
        {"encodings.cp1255", 5482445, 2887},
        {"encodings.cp1256", 5485332, 2865},
        {"encodings.cp1257", 5488197, 2873},
        {"encodings.cp1258", 5491070, 2871},
        {"encodings.cp424", 5493941, 2859},
        {"encodings.cp437", 5496800, 8064},
        {"encodings.cp500", 5504864, 2829},
        {"encodings.cp720", 5507693, 2926},
        {"encodings.cp737", 5510619, 8292},
        {"encodings.cp775", 5518911, 8078},
        {"encodings.cp850", 5526989, 7811},
        {"encodings.cp852", 5534800, 8080},
        {"encodings.cp855", 5542880, 8261},
        {"encodings.cp856", 5551141, 2891},
        {"encodings.cp857", 5554032, 7801},
        {"encodings.cp858", 5561833, 7781},
        {"encodings.cp860", 5569614, 8047},
        {"encodings.cp861", 5577661, 8058},
        {"encodings.cp862", 5585719, 8193},
        {"encodings.cp863", 5593912, 8058},
        {"encodings.cp864", 5601970, 8189},
        {"encodings.cp865", 5610159, 8058},
        {"encodings.cp866", 5618217, 8293},
        {"encodings.cp869", 5626510, 8105},
        {"encodings.cp874", 5634615, 2957},
        {"encodings.cp875", 5637572, 2826},
        {"encodings.cp932", 5640398, 1756},
        {"encodings.cp949", 5642154, 1756},
        {"encodings.cp950", 5643910, 1756},
        {"encodings.euc_jis_2004", 5645666, 1812},
        {"encodings.euc_jisx0213", 5647478, 1812},
        {"encodings.euc_jp", 5649290, 1764},
        {"encodings.euc_kr", 5651054, 1764},
        {"encodings.gb18030", 5652818, 1772},
        {"encodings.gb2312", 5654590, 1764},
        {"encodings.gbk", 5656354, 1740},
        {"encodings.hex_codec", 5658094, 3781},
        {"encodings.hp_roman8", 5661875, 4112},
        {"encodings.hz", 5665987, 1732},
        {"encodings.idna", 5667719, 6368},
        {"encodings.iso2022_jp", 5674087, 1801},
        {"encodings.iso2022_jp_1", 5675888, 1817},
        {"encodings.iso2022_jp_2", 5677705, 1817},
        {"encodings.iso2022_jp_2004", 5679522, 1841},
        {"encodings.iso2022_jp_3", 5681363, 1817},
        {"encodings.iso2022_jp_ext", 5683180, 1833},
        {"encodings.iso2022_kr", 5685013, 1801},
        {"encodings.iso8859_1", 5686814, 2868},
        {"encodings.iso8859_10", 5689682, 2883},
        {"encodings.iso8859_11", 5692565, 2977},
        {"encodings.iso8859_13", 5695542, 2886},
        {"encodings.iso8859_14", 5698428, 2904},
        {"encodings.iso8859_15", 5701332, 2883},
        {"encodings.iso8859_16", 5704215, 2885},
        {"encodings.iso8859_2", 5707100, 2868},
        {"encodings.iso8859_3", 5709968, 2875},
        {"encodings.iso8859_4", 5712843, 2868},
        {"encodings.iso8859_5", 5715711, 2869},
        {"encodings.iso8859_6", 5718580, 2913},
        {"encodings.iso8859_7", 5721493, 2876},
        {"encodings.iso8859_8", 5724369, 2907},
        {"encodings.iso8859_9", 5727276, 2868},
        {"encodings.johab", 5730144, 1756},
        {"encodings.koi8_r", 5731900, 2890},
        {"encodings.koi8_u", 5734790, 2876},
        {"encodings.latin_1", 5737666, 2283},
        {"encodings.mac_arabic", 5739949, 8014},
        {"encodings.mac_centeuro", 5747963, 2937},
        {"encodings.mac_croatian", 5750900, 2945},
        {"encodings.mac_cyrillic", 5753845, 2935},
        {"encodings.mac_farsi", 5756780, 2849},
        {"encodings.mac_greek", 5759629, 2889},
        {"encodings.mac_iceland", 5762518, 2928},
        {"encodings.mac_latin2", 5765446, 4907},
        {"encodings.mac_roman", 5770353, 2906},
        {"encodings.mac_romanian", 5773259, 2946},
        {"encodings.mac_turkish", 5776205, 2929},
        {"encodings.palmos", 5781153, 3066},
        {"encodings.ptcp154", 5784219, 4890},
        {"encodings.punycode", 5789109, 7942},
        {"encodings.quopri_codec", 5797051, 3647},
        {"encodings.raw_unicode_escape", 5800698, 2202},
        {"encodings.rot_13", 5802900, 3656},
        {"encodings.shift_jis", 5806556, 1788},
        {"encodings.shift_jis_2004", 5808344, 1828},
        {"encodings.shift_jisx0213", 5810172, 1828},
        {"encodings.string_escape", 5812000, 2061},
        {"encodings.tis_620", 5814061, 2938},
        {"encodings.undefined", 5816999, 2589},
        {"encodings.unicode_escape", 5819588, 2150},
        {"encodings.unicode_internal", 5821738, 2176},
        {"encodings.utf_16", 5823914, 5160},
        {"encodings.utf_16_be", 5829074, 1990},
        {"encodings.utf_16_le", 5831064, 1990},
        {"encodings.utf_32", 5833054, 5724},
        {"encodings.utf_32_be", 5838778, 1883},
        {"encodings.utf_32_le", 5840661, 1883},
        {"encodings.utf_7", 5842544, 1883},
        {"encodings.utf_8", 5844427, 1942},
        {"encodings.utf_8_sig", 5846369, 4977},
        {"encodings.uu_codec", 5851346, 4909},
        {"encodings.zlib_codec", 5856255, 4641},
        {"functools", 5966425, 6561},
        {"locale", 6591780, 56524},
        {"quopri", 8113272, 6544},
        {"re", 8119816, 13363},
        {"sre_compile", 8382651, 12522},
        {"sre_constants", 8395173, 6177},
        {"sre_parse", 8401350, 21076},
        {"string", 8457791, 20349},
        {"stringprep", 8490627, 14439},
        {"struct", 8505066, 229},
        {"types", 2897374, 2703},
        {NULL, 0, 0}
    };

    struct frozen_desc *current = frozen_modules;

    for(;;) {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
