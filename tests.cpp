#include <gtest/gtest.h>
#include <string>
#include "utils.hpp"

//XOR Encrypt test
TEST(XorEncryptDecryptTest, SameLength) {
    std::string input = "Hello";
    std::string key = "Key123";
    std::string encrypted = xorEncryptDecrypt(input, key);
    EXPECT_EQ(xorEncryptDecrypt(encrypted, key), input);
}

TEST(SortWordsVTest, EmptyVector) {
    std::vector<std::string> words;
    sortWordsV(words);
    EXPECT_TRUE(words.empty());
}

TEST(SortWordsVTest, SingleWordVector) {
    std::vector<std::string> words = {"apple"};
    sortWordsV(words);
    EXPECT_EQ(words.size(), 1);
    EXPECT_EQ(words[0], "apple");
}

TEST(SortWordsVTest, MultipleWordsVector) {
    std::vector<std::string> words = {"Apple", "banana", "cat", "Dog", "Elephant"};
    sortWordsV(words);
    EXPECT_EQ(words.size(), 5);
    EXPECT_EQ(words[0], "Apple");
    EXPECT_EQ(words[1], "banana");
    EXPECT_EQ(words[2], "cat");
    EXPECT_EQ(words[3], "Dog");
    EXPECT_EQ(words[4], "Elephant");
}

TEST(WriteAndReadToDBTest, OneTableInDB){
    fstream db("dbdata/utest.bin", ios::out | ios::binary);
    dbTable inputTable;
    inputTable.secret = "hash";
    inputTable.words = {"Adrian", "test", "dlugie zdanie", "Zenon", "raz"};
    vector<string> expected = {"Adrian", "dlugie zdanie", "raz", "test", "Zenon"};
    writeTableToDB(db, inputTable);
    db.close();
    fstream dbRead("dbdata/utest.bin", ios::in | ios::binary);
    dbTable readTable = readTableFromDB(dbRead, inputTable.secret);
    dbRead.close();
    EXPECT_EQ(readTable.words.size(), 5);
    EXPECT_EQ(readTable.words[0], expected[0]);
    EXPECT_EQ(readTable.words[1], expected[1]);
    EXPECT_EQ(readTable.words[2], expected[2]);
    EXPECT_EQ(readTable.words[3], expected[3]);
    EXPECT_EQ(readTable.words[4], expected[4]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}