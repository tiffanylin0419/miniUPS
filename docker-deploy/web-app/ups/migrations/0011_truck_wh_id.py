# Generated by Django 4.2 on 2023-04-24 02:28

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0010_package_amazon_user_name'),
    ]

    operations = [
        migrations.AddField(
            model_name='truck',
            name='wh_id',
            field=models.IntegerField(db_column='wh_id', default=None),
        ),
    ]
