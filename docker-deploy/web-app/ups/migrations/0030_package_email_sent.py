# Generated by Django 4.2 on 2023-04-28 02:08

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0029_alter_package_addr_x_alter_package_addr_y'),
    ]

    operations = [
        migrations.AddField(
            model_name='package',
            name='email_sent',
            field=models.BooleanField(default=False),
        ),
    ]
