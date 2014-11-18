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

from sqlalchemy.orm.exc import MultipleResultsFound
from sqlalchemy.orm.exc import NoResultFound

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

    # tune
    #engine.echo = True
    #engine.  # хотел отформатировать, но так можно в Hibernate non here
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
# !! http://stackoverflow.com/questions/4209705/is-it-worth-using-sqlalchemy-migrate
#   https://sqlalchemy-migrate.readthedocs.org/en/latest/
class TaskEntity(Base):
    __tablename__ = 'task_entity'
    id = Column(Integer, primary_key=True)

    feature = Column(String)
    task_name = Column(String)

    priority = Column(String)
    done = Column(Boolean)

    tags = relationship('Tag', secondary='task_tag_link', backref='task_entity')


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

    tag_name = 'project'
    with session_scope(Session) as session:
        # add task and tag

        tag = None
        try:
            #session.query(Tag).filter(Tag.name.has(tag_name))  # !(this is it)
            tag = session.query(Tag).filter(Tag.name == tag_name).one()
        except NoResultFound, e:
            tag = Tag(name=tag_name)
            session.add(tag)

        except MultipleResultsFound, e:
            raise  # FIXME: make own

        for instance in session.query(TaskEntity) \
                .filter(
                and_(TaskEntity.task_name.match("*VCard*"),
                     TaskEntity.done == False)):
            tag.tasks.append(instance)
            instance.tags = [tag]
            print instance.tags, instance.priority, instance.task_name, instance.feature

    # Select by tag
    # http://stackoverflow.com/questions/4679846/several-many-to-many-table-joins-with-sqlalchemy
    # http://www.pythoncentral.io/sqlalchemy-orm-examples/
    # http://www.lornajane.net/posts/2011/inner-vs-outer-joins-on-a-many-to-many-relationship
    with session_scope(Session) as session:
        # just Tag no way
        # "knows how to join between User and Address because there’s only one foreign key between "
        for instance in session.query(TaskEntity).join(Tag, TaskEntity.tags).filter(Tag.name == tag_name).all():
            print instance.task_name


if __name__ == '__main__':
    main()