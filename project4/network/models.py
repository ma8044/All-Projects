from django.contrib.auth.models import AbstractUser
from django.db import models


class User(AbstractUser):
    following = models.ManyToManyField('self', symmetrical=False, related_name='followers')

class Post(models.Model):
    creator = models.ForeignKey(User, on_delete=models.CASCADE, related_name="posts")
    body = models.TextField(blank=True)
    likes = models.ManyToManyField(User, related_name="posts_liked")
    timestamp = models.DateTimeField(auto_now_add=True)
    edit =  models.BooleanField(default=False)

    def serialize(self):
        return {
            "id": self.id,
            "creator": self.creator.username,
            "creatorid": self.creator.pk,
            "body": self.body,
            "timestamp": self.timestamp.strftime("%b %d %Y, %I:%M %p"),
            "likes": [user.username for user in self.likes.all()],
            "edit": self.edit
        }