# coding: utf-8
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, Boolean
from sqlalchemy.orm import sessionmaker
from contextlib import contextmanager

# relations
from sqlalchemy import ForeignKey
from sqlalchemy.orm import relationship, backref
from sqlalchemy import and_

Base = declarative_base()  # it's class?!


def build_engine():
    # engine config
    DB_URI = "postgresql+psycopg2://{user}:{password}@{host}:{port}/{db}"
    engine = create_engine(DB_URI.format(
        user='postgres',
        password='123',
        host='127.0.0.1',
        port=5432,
        db='postgres'))
    # , connect_args={'time_zone': '+00:00'} - failed

    # now connected
    #engine.echo = True
    return engine


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

# Migration
#  !! http://stackoverflow.com/questions/4209705/is-it-worth-using-sqlalchemy-migrate
#   https://sqlalchemy-migrate.readthedocs.org/en/latest/
class TaskEntity(Base):
    __tablename__ = 'task_entity'
    id = Column(Integer, primary_key=True)

    feature = Column(String)
    task_name = Column(String)

    priority = Column(String)
    done = Column(Boolean)

    tags = relationship('Tag', secondary='task_tag_link')

class Tag(Base):
    __tablename__ = 'tags'
    id = Column(Integer, primary_key=True)

    name = Column(String)
    color = Column(String)

    tasks = relationship(TaskEntity, secondary='task_tag_link')


class TaskTagLink(Base):
    __tablename__ = 'task_tag_link'
    # !! naming is strong here
    task_entity_id = Column(Integer, ForeignKey('task_entity.id'), primary_key=True)
    tags_id = Column(Integer, ForeignKey('tags.id'), primary_key=True)

def main():
    engine = build_engine()
    Session = sessionmaker(bind=engine)  # it's class!
    Base.metadata.create_all(engine)

    with session_scope(Session) as session:
        # add task and tag

        for instance in session.query(TaskEntity)\
                .filter(and_(TaskEntity.task_name.match("*VCard*"), TaskEntity.done == False)):
            print instance.tags, instance.priority, instance.task_name, instance.feature


if __name__ == '__main__':
    main()