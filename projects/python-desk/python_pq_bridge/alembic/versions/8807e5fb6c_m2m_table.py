"""m2m table

Revision ID: 8807e5fb6c
Revises: 392e1a7038a5
Create Date: 2014-11-18 14:38:49.152000

"""

# revision identifiers, used by Alembic.
revision = '8807e5fb6c'
down_revision = '392e1a7038a5'

from alembic import op
import sqlalchemy as sa


def upgrade():
    op.create_table('tags',
        sa.Column('id', sa.Integer, primary_key=True),
        sa.Column('name', sa.String),
        sa.Column('color', sa.String)
    )

    op.create_table(
        'task_tag_link',
        sa.Column(
            'task_entity_id', sa.Integer,
            sa.ForeignKey('task_entity.id'), primary_key=True
        ),
        sa.Column(
            'tags_id', sa.Integer,
            sa.ForeignKey('tags.id'), primary_key=True
        )
    )


def downgrade():
    op.drop_table(
        'task_tag_link'
    )
    op.drop_table(
        'tags'
    )
