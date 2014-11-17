"""create account table

Revision ID: 74d89c0efb3
Revises: None
Create Date: 2014-11-17 16:24:22.197000

"""

# revision identifiers, used by Alembic.
revision = '74d89c0efb3'
down_revision = None

from alembic import op
import sqlalchemy as sa


def upgrade():
    op.create_table(
        'account',
        sa.Column('id', sa.Integer, primary_key=True),
        sa.Column('name', sa.String(50), nullable=False),
        sa.Column('description', sa.Unicode(200)),
    )

def downgrade():
    op.drop_table('account')