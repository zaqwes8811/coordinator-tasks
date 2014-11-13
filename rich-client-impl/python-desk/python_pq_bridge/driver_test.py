# coding: utf-8
#!/usr/bin/python2.4
#
# Small script to show PostgreSQL and Pyscopg together
#
# https://wiki.postgresql.org/wiki/Psycopg2_Tutorial
#
# Notes:
#  с транзакциями не так то просто. http://initd.org/psycopg/docs/connection.html#connection.autocommit
# http://stackoverflow.com/questions/5402805/error-when-creating-a-postgresql-database-using-python-sqlalchemy-and-psycopg2
# http://stackoverflow.com/questions/3413646/postgres-raises-a-active-sql-transaction-errcode-25001
# http://www.devx.com/opensource/Article/29071

import psycopg2

# PostgreSQL/C++/VS08/Win7/x64 - looks very hard
#   http://qt-project.org/doc/qt-5/qsqldatabase.html
#   http://qt-project.org/doc/qt-5/sql-driver.html#supported-databases
#
# http://www.youtube.com/watch?v=fBgJ9Azm_S0 - похоже нужен вариант с MinGW - WTF!? а остальные библиотеки!
#   может и для студии нормально соберется?
# http://www.carnero.net/en/how-to-install-qt-mysql-driver-on-windows-7-x86-x64/

'''
Joel - 2008

// text
feature
task
note

// int
priority

// in hours
// FIXME: не ясно как рассчитывать - пока просто резерв
original_estimation
current_estimation
elapsed
remain
'''

cur = None
conn = None
table_name = "task_entity"
schedule_elem_entity = "schedule_elem_entity"
try:
    conn = psycopg2.connect("dbname='postgres' user='postgres' host='localhost' password='123'")
    conn.set_session(autocommit=True)

    # Clear what done
    cur = conn.cursor()

    cur.execute("CREATE TABLE " +
                "IF NOT EXISTS " +  # v9.1 >=
                schedule_elem_entity +
                "(" +  # // сделать чтобы было >0
                "ID         SERIAL PRIMARY KEY NOT NULL," +
                "TASK_NAME  TEXT               NOT NULL, " +
                "PRIORITY   INT                NOT NULL, " +
                "DONE BOOLEAN DEFAULT FALSE);")

    # Queries
    cur.execute("SELECT * from " + table_name)
    rows = cur.fetchall()
    print "\nRows: \n"
    for row in rows:
        print " ", row[1]

    # Remove
    #conn.set_isolation_level(0)  # Old
    #drop_cur = conn.cursor()
    cur.execute("DROP TABLE " + schedule_elem_entity) # не видит таблицу
    #conn.set_isolation_level(1)

except psycopg2.OperationalError, e:
    print "I am unable to connect to the database", e
except psycopg2.InternalError, e:
    print "I can't drop our test database!", e
# except psycopg2.ProgrammingError, e:
# База была создана
# print e

finally:
    if cur:
        cur.close()
    if conn:
        conn.close()

