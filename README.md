# THJDB

An efficient and powerful relational database management system written in C++,
compatible with Python interface.
This project is a class assignment for Tsinghua Database courses and there would be no warranty that this project will be maintained later. Any contributions are welcome though.

## SQL Support
THJDB supports the SQLServer style SQL. Here are some commands that THJDB supports.

    CREATE DATABASE student;
    SHOW DATABASES;
    USE student;
    SHOW TABLES;
    CREATE TABLE grade (id int(10), age time(), height double(), name varchar(100), primary key id, teacher_id int(3) foreign key references teacher(id));
    CREATE INDEX grade(id);
    SELECT grade.id, teacher.age FROM grade, teacher WHERE grade.teacher_id = teacher.id;
    SELECT * FROM grade WHERE name LIKE '%ohn' AND age > 13;
    SELECT age, AVG(height) from grade GROUP BY age;
    CREATE MASTER KEY ENCRYPTION BY PASSWORD = '123'；

The DB Management system supports joining and grouping algorithm for various combinations.
Currently the nested-loop join, merge-join and hash-join algorithms are implemented.

An UI based on Qt is provided for convenient usage.

## Usage

Once compiled, THJDB can be used as follows:

- Command line mode: enter `./database cmd` to trigger this mode. A text-based interactive shell is provided for SQL input and result output.
- UI: double click on the binary file to run in graphics mode. A modern-design UI will guide you through the management system.
- Python interface: use `import thjdb`, `thjdb.open()`, `thjdb.execSQL()` and `thjdb.close()` to manage the database. All results are returned in json format and can be easily interpreted by Python `json.loads()` method.

## How to compile

Run this first before compiling a GUI version of THJDB

    git clone https://github.com/openssl/openssl.git
    cd openssl
    git checkout OpenSSL_1_0_0-stable
    ./config enable-shared
    make depend
    make -j4
    sudo cp libcrypto.so libssl.so libcrypto.so.1.0.0 libssl.so.1.0.0 /opt/Qt/5.7/gcc_64/lib/

The project is made with Qt Creator and QMake. Simply open `database/database.pro` and click compile. (Make sure bison is installed on your computer if you are running a Linux OS)

For Python interface, open `builddatabase/builddatabase.pro` and click comile in Qt Creator.

## Acknowledgements

We (Fangze Guo, Jiahui Huang, Zhongyu Jiang) thank Professor Feng and all the TAs for all their devotions in this semester.
