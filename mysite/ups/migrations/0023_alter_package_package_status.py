# Generated by Django 4.2 on 2023-04-24 19:37

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ups', '0022_truck_wh_id'),
    ]

    operations = [
        migrations.AlterField(
            model_name='package',
            name='package_status',
            field=models.CharField(choices=[('pickup', 'pickup'), ('loading', 'loading'), ('delivering', 'delivering'), ('delivered', 'delivered')], default='Pickup', max_length=10),
        ),
    ]
