# Generated by Django 4.2 on 2023-04-24 20:15

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0023_alter_package_package_status'),
    ]

    operations = [
        migrations.AlterField(
            model_name='truck',
            name='truck_status',
            field=models.CharField(choices=[('idle', 'Idle'), ('traveling', 'traveling'), ('loading', 'loading'), ('delivering', 'delivering')], max_length=20),
        ),
    ]
