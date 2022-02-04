#include "catch.hpp"
#include "QPS/PQLTable.h"
#include "QPS/Row.h"

using namespace std;

TEST_CASE() {
    Header header({"a", "b", "c", "d"});
    PQLTable table = PQLTable(header);
    Row *row = new Row();
    row->addEntry("a", "1");
    row->addEntry("b", "2");
    row->addEntry("c", "3");
    row->addEntry("d", "4");

    Row *row2 = new Row();
    row2->addEntry("a", "1");
    row2->addEntry("b", "2");
    row2->addEntry("c", "3");
    row2->addEntry("e", "4");

    SECTION("empty table") {
        Header header2({"a", "b", "c", "d"});

        REQUIRE(table.isEmpty());
        REQUIRE(table.size() == 0);
        REQUIRE_FALSE(table.hasColumn("e"));
        REQUIRE(table.hasColumn("a"));
        REQUIRE(table.getHeader() == header2);
    }

    SECTION("populated table") {
        Header header2({"a", "b", "c", "d"});
        REQUIRE_NOTHROW(table.addRow(row));
        REQUIRE_THROWS(table.addRow((row2)));
        REQUIRE_THROWS(table.addRow(nullptr));

        // this is to test if adding rows will affect the checks we have done
        REQUIRE(table.size() == 1);
        REQUIRE_FALSE(table.isEmpty());
        REQUIRE_FALSE(table.hasColumn("e"));
        REQUIRE(table.hasColumn("a"));
        REQUIRE(table.getHeader() == header2);
    }

    SECTION("combine table") {
        Header header2({"a", "v"});
        Header header3({"a"});
        Header header4({"a", "d"});

        /*
         * a | v
         * 1 | a
         * 1 | b
         * 2 | c
         */
        PQLTable table2 = PQLTable(header2);
        Row* row1a = new Row();
        Row* row1b = new Row();
        Row* row2c = new Row();
        REQUIRE_NOTHROW(row1a->addEntry("a", "1"));
        REQUIRE_NOTHROW(row1a->addEntry("v", "a"));
        REQUIRE_NOTHROW(row1b->addEntry("a", "1"));
        REQUIRE_NOTHROW(row1b->addEntry("v", "b"));
        REQUIRE_NOTHROW(row2c->addEntry("a", "2"));
        REQUIRE_NOTHROW(row2c->addEntry("v", "c"));
        REQUIRE_NOTHROW(table2.addRow(row1a));
        REQUIRE_NOTHROW(table2.addRow(row1b));
        REQUIRE_NOTHROW(table2.addRow(row2c));
        REQUIRE_THROWS(table2.mergeJoin(nullptr));
        /*
         * a
         * 1
         * 4
         * 3
         */
        auto* table3 = new PQLTable(header3);
        Row* a1 = new Row();
        Row* a4 = new Row();
        Row* a3 = new Row();

        REQUIRE_NOTHROW(a1->addEntry("a", "1"));
        REQUIRE_NOTHROW(a4->addEntry("a", "4"));
        REQUIRE_NOTHROW(a3->addEntry("a", "3"));
        REQUIRE_NOTHROW(table3->addRow(a1));
        REQUIRE_NOTHROW(table3->addRow(a4));
        REQUIRE_NOTHROW(table3->addRow(a3));

        /*
         * a | v
         * 1 | a
         * 1 | b
         */
        PQLTable* newTable = table2.mergeJoin(table3);
        REQUIRE(newTable->getHeader() == header2);
        REQUIRE(newTable->size() == 2);
        REQUIRE(newTable->hasRow(row1a));
        REQUIRE(newTable->hasRow(row1b));
        REQUIRE_FALSE(newTable->hasRow(row2c));

        /*
         * a | d
         * 1 | z
         * 4 | x
         * 3 | y
         */
        auto* table4 = new PQLTable(header4);
        Row* row1z = new Row();
        Row* row4x = new Row();
        Row* row3y = new Row();
        REQUIRE_NOTHROW(row1z->addEntry("a", "1"));
        REQUIRE_NOTHROW(row1z->addEntry("d", "z"));
        REQUIRE_NOTHROW(row4x->addEntry("a", "4"));
        REQUIRE_NOTHROW(row4x->addEntry("d", "x"));
        REQUIRE_NOTHROW(row3y->addEntry("a", "3"));
        REQUIRE_NOTHROW(row3y->addEntry("d", "y"));
        REQUIRE_NOTHROW(table4->addRow(row1z));
        REQUIRE_NOTHROW(table4->addRow(row4x));
        REQUIRE_NOTHROW(table4->addRow(row3y));

        /*
         * a | v | d
         * 1 | a | z
         * 1 | b | z
         */
        PQLTable* newTable2 = table2.mergeJoin(table4);
        Header header5({"a", "d", "v"});
        REQUIRE(newTable2->getHeader() == header5);
        REQUIRE(newTable2->size() == 2);

        Row* row1az = new Row();
        REQUIRE_NOTHROW(row1az->addEntry("a", "1"));
        REQUIRE_NOTHROW(row1az->addEntry("v", "a"));
        REQUIRE_NOTHROW(row1az->addEntry("d", "z"));
        Row* row1bz = new Row();
        REQUIRE_NOTHROW(row1bz->addEntry("a", "1"));
        REQUIRE_NOTHROW(row1bz->addEntry("v", "b"));
        REQUIRE_NOTHROW(row1bz->addEntry("d", "z"));
        REQUIRE(newTable2->hasRow(row1az));
        REQUIRE(newTable2->hasRow(row1bz));
    }
}