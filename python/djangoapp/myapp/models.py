from django.db import models

# Create your models here.

class Person(models.Model):
    name = models.CharField(max_length=30)
    age = models.IntegerField(null=True)
    sex = models.CharField(max_length=30, null=True, blank=True)
    created = models.DateField(auto_now=True)

    def __unicode__(self):
        return self.name

class Address(models.Model):
    code = models.CharField(max_length=10)
    persons = models.ManyToManyField(Person)

