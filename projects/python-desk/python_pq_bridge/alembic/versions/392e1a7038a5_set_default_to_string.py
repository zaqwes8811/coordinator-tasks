"""set default to string

Revision ID: 392e1a7038a5
Revises: 3e607a3a858c
Create Date: 2014-11-17 17:41:47.983000

"""

# revision identifiers, used by Alembic.
revision = '392e1a7038a5'
down_revision = '3e607a3a858c'

from alembic import op
import sqlalchemy as sa


def upgrade():
    # default ignored 
    op.drop_column('task_entity', 'feature')
    op.add_column('task_entity', sa.Column('feature', sa.String, server_default=""))

    
def downgrade():   
    # default ignored 
    op.drop_column('task_entity', 'feature')
    op.add_column('task_entity', sa.Column('feature', sa.String, server_default=''))