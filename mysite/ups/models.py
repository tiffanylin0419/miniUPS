import datetime
from django.contrib.auth.models import AbstractUser
from django.db import models
from django.utils import timezone
from django.contrib.auth.models import User

from django.views.generic import View



class World(models.Model):
    world_id = models.AutoField(primary_key=True, db_column='world_id')
    connected = models.BooleanField(default=False)

# class User(AbstractUser):
#     amazon_account = models.CharField(max_length=255, blank=True)
#     world = models.ForeignKey(World, on_delete=models.CASCADE, blank=True, null=True)


class Truck(models.Model):
    TRUCK_STATUS_CHOICES = (
        ('idle', 'Idle'),
        ('traveling', 'traveling'),
        ('loading', 'loading'),
        ('delivering', 'delivering'),
    )
    truck_id = models.AutoField(primary_key=True, db_column='truck_id')
    world_id = models.ForeignKey(World, on_delete=models.CASCADE, blank=True, null=True, db_column='world_id')
    truck_status = models.CharField(max_length=20, choices=TRUCK_STATUS_CHOICES)
    loc_x = models.FloatField()
    loc_y = models.FloatField()
    wh_id = models.IntegerField(blank=True, null=True)

    def __str__(self):
        return f"Truck {self.truck_id} in World {self.world_id}"
    

class Package(models.Model):
    PACKAGE_STATUS_CHOICES = [
        ('pickup', 'pickup'),
        ('loading', 'loading'),
        ('delivering', 'delivering'),
        ('delivered', 'delivered'),
    ]

    package_id = models.AutoField(primary_key=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    amazon_user_name = models.CharField(max_length=100, default='')
    wh_id = models.IntegerField()
    addr_y = models.IntegerField()
    addr_x = models.IntegerField()
    truck_id = models.ForeignKey(Truck, on_delete=models.SET_NULL, null=True, db_column='truck_id')
    des_x = models.IntegerField()
    des_y = models.IntegerField()
    package_status = models.CharField(max_length=10, choices=PACKAGE_STATUS_CHOICES, default='Pickup')
    world_id = models.ForeignKey(World, on_delete=models.CASCADE, db_column='world_id')
    ready_for_picktime = models.DateTimeField(null=True, blank=True)
    load_time = models.DateTimeField(null=True, blank=True)
    delivered_time = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return f"Package {self.package_id}"
    


 

