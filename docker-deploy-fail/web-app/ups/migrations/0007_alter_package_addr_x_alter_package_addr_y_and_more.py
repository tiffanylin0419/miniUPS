# Generated by Django 4.2 on 2023-04-20 18:20

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0006_delete_user'),
    ]

    operations = [
        migrations.AlterField(
            model_name='package',
            name='addr_x',
            field=models.IntegerField(),
        ),
        migrations.AlterField(
            model_name='package',
            name='addr_y',
            field=models.IntegerField(),
        ),
        migrations.AlterField(
            model_name='package',
            name='des_x',
            field=models.IntegerField(),
        ),
        migrations.AlterField(
            model_name='package',
            name='des_y',
            field=models.IntegerField(),
        ),
    ]
