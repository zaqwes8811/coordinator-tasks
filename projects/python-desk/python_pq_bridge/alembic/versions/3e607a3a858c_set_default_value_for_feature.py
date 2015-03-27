"""set default value for feature

Revision ID: 3e607a3a858c
Revises: 330d1836f4a4
Create Date: 2014-11-17 17:31:42.970000

"""

# revision identifiers, used by Alembic.
revision = '3e607a3a858c'
down_revision = '330d1836f4a4'

from alembic import op
import sqlalchemy as sa


def upgrade():
    # default ignored 
    op.drop_column('task_entity', 'feature')
    op.add_column('task_entity', sa.Column('feature', sa.String, server_default=''))


def downgrade():
    op.drop_column('task_entity', 'feature')
    op.add_column('task_entity', sa.Column('feature', sa.String))