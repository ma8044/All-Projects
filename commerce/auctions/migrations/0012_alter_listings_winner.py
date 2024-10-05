# Generated by Django 5.0.6 on 2024-07-13 12:56

import django.db.models.deletion
from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('auctions', '0011_listings_onwatch_listings_winner'),
    ]

    operations = [
        migrations.AlterField(
            model_name='listings',
            name='winner',
            field=models.ForeignKey(blank=True, default=None, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='won', to=settings.AUTH_USER_MODEL),
        ),
    ]
