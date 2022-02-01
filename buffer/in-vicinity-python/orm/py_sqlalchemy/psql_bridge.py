# coding: utf-8
# https://realpython.com/blog/python/flask-by-example-part-2-postgres-sqlalchemy-and-alembic/
#
# Tut.:
# http://www.rmunn.com/sqlalchemy-tutorial/tutorial.html
# http://docs.sqlalchemy.org/en/rel_0_9/orm/tutorial.html
#
# http://docs.sqlalchemy.org/en/latest/core/engines.html#postgresql
#
import sqlalchemy as sa
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import sessionmaker
from contextlib import contextmanager

# relations
from sqlalchemy import ForeignKey
from sqlalchemy.orm import relationship, backref

# sys
import unittest


def build_engine():
    # engine config
    DB_URI = "postgresql+psycopg2://{user}:{password}@{host}:{port}/{db}"
    engine = create_engine(DB_URI.format(
        user='postgres',
        password='123',
        host='127.0.0.1',
        port=5432,
        db='testdb'))
    # , connect_args={'time_zone': '+00:00'} - failed

    # now connected
    # engine.echo = True
    return engine


# Rules
# http://docs.sqlalchemy.org/en/rel_0_9/orm/session.html
# as in hibernate, but    
@contextmanager
def session_scope(Session):
    """Provide a transactional scope around a series of operations."""
    session = Session()
    try:
        yield session
        session.commit()
    except:
        session.rollback()
        raise
    finally:
        session.close()


class ORMTest(unittest.TestCase):
    def test_relations(self):
        Base = declarative_base()  # it's class?!

        # One-to-many
        class User(Base):  # parent
            # переменный как бы статические, но похоже потом преобразуются
            __tablename__ = 'users'
            id = Column(Integer, primary_key=True)
            name = Column(String)
            fullname = Column(String)
            password = Column(String)

            addresses = relationship("Address", cascade="all, delete, delete-orphan", backref='users')  # childes

            def get_addresses(self):
                return self.addresses

            def __repr__(self):
                return "<User(name='%s', fullname='%s', password='%s'" % (
                    self.name, self.fullname, self.password)

        class Address(Base):  # child
            __tablename__ = 'addressess'
            id = Column(Integer, primary_key=True)
            email_address = Column(String, nullable=False)

            # in child
            user_id = Column(Integer, ForeignKey('users.id'))

            def __repr__(self):
                return "<Address(email_address='%s')>" % self.email_address

        engine = build_engine()
        Session = sessionmaker(bind=engine)  # it's class!
        Base.metadata.create_all(engine)

        try:
            with session_scope(Session) as session:
                session.add_all([
                    User(name='wendy', fullname='Wendy Williams', password='foobar'),
                    User(name='mary', fullname='Mary Contrary', password='xxg527'),
                    User(name='fred', fullname='Fred Flinstone', password='blah')])

            with session_scope(Session) as session:
                jack = User(name='jack', fullname='Jack Bean', password='gjffdd')
                print jack.addresses
                jack.addresses = [
                    Address(email_address='jack@google.com'),
                    Address(email_address='j25@yahoo.com')]

                # store
                session.add(jack)

            with session_scope(Session) as session:
                # queries
                jack = session.query(User).filter_by(name='jack').one()
                print jack
                print jack.get_addresses()  # lazy loading
                # session.delete(jack)
                print session.query(User).join(Address). \
                    filter(Address.email_address == 'jack@google.com'). \
                    all()

        finally:
            with session_scope(Session) as session:
                # not deleted if exist childes
                # jack = session.query(User).filter_by(name='jack').one()
                # session.delete(jack)
                session.query(Address).delete()  # сперва их! иначе не удалятся
                session.query(User).delete()
                session.flush()

    def test_m2m(self):
        Base = declarative_base()  # it's class?!

        post_keywords = sa.Table('post_keywords', Base.metadata,
                                 Column('post_id', Integer, ForeignKey('posts.id')),
                                 Column('keyword_id', Integer, ForeignKey('keywords.id'))
        )

        class BlogPost(Base):
            __tablename__ = 'posts'

            id = Column(Integer, primary_key=True)
            # user_id = Column(Integer, ForeignKey('users.id'))
            headline = Column(String(255), nullable=False)
            body = Column(sa.Text)

            # many to many BlogPost<->Keyword
            keywords = relationship('Keyword', secondary=post_keywords, backref='posts')
            #author = relationship(User, backref=backref('posts', lazy='dynamic'))

            def __init__(self, headline, body, author):
                self.author = author
                self.headline = headline
                self.body = body

            def __repr__(self):
                return "BlogPost(%r, %r, %r)" % (self.headline, self.body, self.author)

        class Keyword(Base):
            __tablename__ = 'keywords'

            id = Column(Integer, primary_key=True)
            keyword = Column(String(50), nullable=False, unique=True)

            def __init__(self, keyword):
             self.keyword = keyword

        engine = build_engine()
        Session = sessionmaker(bind=engine)  # it's class!
        Base.metadata.create_all(engine)

    def test_simple_object(self):
        Base = declarative_base()  # it's class?!

        # Many-to-One - don't work - или это было one-to-one
        # One-to-Many
        # http://docs.sqlalchemy.org/en/rel_0_9/orm/relationships.html
        class User(Base):
            __tablename__ = 'users'
            id = Column(Integer, primary_key=True)
            name = Column(String)
            fullname = Column(String)
            password = Column(String)

            # cyrc. depen. occure
            # address_id = Column(Integer, ForeignKey('addressess.id'))

            # step 2
            # FIXME: don't work
            # addresses = relationship("Address")#, backref=backref('user', order_by=id))

            def __repr__(self):
                return "<User(name='%s', fullname='%s', password='%s'" % (
                    self.name, self.fullname, self.password)

        engine = build_engine()
        Session = sessionmaker(bind=engine)  # it's class!
        Base.metadata.create_all(engine)

        with session_scope(Session) as session:
            try:
                # create objects
                ed_user = User(name='ed', fullname='Ed Jones', password='123')
                assert not ed_user.id  # transient object
                # session = Session()  # now really connect
                # raise Exception()

                session.add(ed_user)  # pending..
                ed_user.password = '321'
                print session.dirty, session.new
                session.commit()
                print session.dirty, session.new
                ed_user.password = '321'
                print session.dirty, session.new
                print ed_user in session
                # session.commit()  # if dirty is non empty then on delete deadlock - if new session created

                # Querying
                for name, fullname in session.query(User.name, User.fullname):
                    print name, fullname

                    # Relations
            except:
                if session:
                    session.rollback()
                raise

            finally:
                if session:
                    session.commit()

                session.query(User).delete()
                session.flush()
                session.commit()
                session.close()