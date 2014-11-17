"""add column

Revision ID: 4b54778d05ca
Revises: 73abafc351c
Create Date: 2014-11-17 16:44:36.214000

"""

# revision identifiers, used by Alembic.
revision = '4b54778d05ca'
down_revision = '73abafc351c'

from alembic import op
import sqlalchemy as sa


def upgrade():
    # May be danger! best make change query
	#op.add_column('task_entity', sa.Column('assign', sa.String, default='I'))
	#for ...:
	#	if not assign:
	#		set empty string
	pass


def downgrade():
    pass
	#op.drop_column('task_entity', 'assign')
