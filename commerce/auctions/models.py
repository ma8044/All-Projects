from django.contrib.auth.models import AbstractUser
from django.db import models


class User(AbstractUser):
    pass

class Listings(models.Model):
    title = models.CharField(max_length=64)
    description = models.CharField(max_length=500)
    creator = models.ForeignKey(User, on_delete=models.CASCADE, related_name="listings")
    img = models.CharField(max_length=500, blank=True, null=True)
    startBid = models.IntegerField()
    category = models.CharField(max_length=64, blank=True, null=True)
    active = models.BooleanField(default=True)
    winner = models.ForeignKey(User, on_delete=models.CASCADE, related_name="won", default=None, blank=True, null=True)
    onwatch = models.ManyToManyField(User, blank=True, related_name="watchlist")

class Bids(models.Model):
    creator = models.ForeignKey(User, on_delete=models.CASCADE, related_name="bids")
    bid = models.IntegerField()
    listing = models.ForeignKey(Listings, on_delete=models.CASCADE, related_name="bids", blank=True, null=True )

class Comments(models.Model):
    creator = models.ForeignKey(User, on_delete=models.CASCADE, related_name="comments")
    comment = models.CharField(max_length=300)
    listing = models.ForeignKey(Listings, on_delete=models.CASCADE, related_name="comments", blank=True, null=True)
