# Generated by Django 4.2 on 2023-04-18 22:11

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0003_user_truck_package'),
    ]

    operations = [
        migrations.DeleteModel(
            name='User',
        ),
    ]