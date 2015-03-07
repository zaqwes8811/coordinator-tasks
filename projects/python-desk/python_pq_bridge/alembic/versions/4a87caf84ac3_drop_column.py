"""drop column

Revision ID: 4a87caf84ac3
Revises: None
Create Date: 2014-11-17 17:09:14.664000

"""

# revision identifiers, used by Alembic.
revision = '4a87caf84ac3'
down_revision = None

from alembic import op
import sqlalchemy as sa


def upgrade():
    op.drop_column('task_entity', 'assign')


def downgrade():
    op.add_column('task_entity', sa.Column('assign', sa.String))
