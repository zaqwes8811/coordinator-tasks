"""add feature table

Revision ID: 330d1836f4a4
Revises: 4a87caf84ac3
Create Date: 2014-11-17 17:14:37.437000

"""

# revision identifiers, used by Alembic.
revision = '330d1836f4a4'
down_revision = '4a87caf84ac3'

from alembic import op
import sqlalchemy as sa


def upgrade():
    # default ignored 
    op.add_column('task_entity', sa.Column('feature', sa.String, default=''))


def downgrade():
    op.drop_column('task_entity', 'feature')
