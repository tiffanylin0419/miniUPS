# Generated by Django 4.2 on 2023-04-24 02:45

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0018_alter_package_wh_id_alter_truck_wh_id'),
    ]

    operations = [
        migrations.AlterField(
            model_name='truck',
            name='wh_id',
            field=models.IntegerField(blank=True, null=True),
        ),
    ]
