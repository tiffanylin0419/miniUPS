import datetime
from django.contrib.auth.models import AbstractUser
from django.db import models
from django.utils import timezone
from django.contrib.auth.models import User

from django.views.generic import View



class World(models.Model):
    world_id = models.AutoField(primary_key=True)
    connected = models.BooleanField(default=False)

# class User(AbstractUser):
#     amazon_account = models.CharField(max_length=255, blank=True)
#     world = models.ForeignKey(World, on_delete=models.CASCADE, blank=True, null=True)


class Truck(models.Model):
    TRUCK_STATUS_CHOICES = (
        ('idle', 'Idle'),
        ('traveling', 'Traveling'),
        ('arrive_warehouse', 'Arrive Warehouse'),
        ('loading', 'Loading'),
        ('delivering', 'Delivering'),
    )
    truck_id = models.AutoField(primary_key=True)
    world_id = models.ForeignKey(World, on_delete=models.CASCADE, blank=True, null=True)
    truck_status = models.CharField(max_length=20, choices=TRUCK_STATUS_CHOICES)
    loc_x = models.FloatField()
    loc_y = models.FloatField()

    def __str__(self):
        return f"Truck {self.truck_id} in World {self.world_id}"
    

class Package(models.Model):
    PACKAGE_STATUS_CHOICES = [
        ('Pickup', 'Pickup'),
        ('Delivering', 'Delivering'),
        ('Delivered', 'Delivered'),
    ]

    package_id = models.AutoField(primary_key=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    wh_id = models.IntegerField()
    addr_y = models.FloatField()
    addr_x = models.FloatField()
    truck_id = models.ForeignKey(Truck, on_delete=models.SET_NULL, null=True)
    des_x = models.FloatField()
    des_y = models.FloatField()
    package_status = models.CharField(max_length=10, choices=PACKAGE_STATUS_CHOICES, default='Pickup')
    world_id = models.ForeignKey(World, on_delete=models.CASCADE)
    ready_for_picktime = models.DateTimeField(null=True, blank=True)
    load_time = models.DateTimeField(null=True, blank=True)
    delivered_time = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return f"Package {self.package_id}"
    


 

