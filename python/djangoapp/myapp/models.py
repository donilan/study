from django.db import models

# Create your models here.

class Person(models.Model):
    name = models.CharField(max_length=30)

    def __unicode__(self):
        return self.name

class Address(models.Model):
    code = models.CharField(max_length=10)
    persons = models.ManyToManyField(Person)

class Group(models.Model):
    name = models.CharField(max_length=128)
    members = models.ManyToManyField(Person, through='Membership')

    def __unicode__(self):
        return self.name

class Membership(models.Model):
    person = models.ForeignKey(Person)
    group = models.ForeignKey(Group)
    date_joined = models.DateField()
    invite_reason = models.CharField(max_length=64)

class Musician(models.Model):
    first_name = models.CharField(max_length=50)
    last_name = models.CharField(max_length=50)
    instrument = models.CharField(max_length=100)

class Album(models.Model):
    artist = models.ForeignKey(Musician)
    name = models.CharField(max_length=100)
    release_date = models.DateField()
    num_stars = models.IntegerField()

class CommonInfo(models.Model):
    name = models.CharField(max_length=100)
    age = models.PositiveIntegerField()
    created = models.DateField(auto_now=False, auto_now_add=True)
    person = models.ManyToManyField(Person, related_name="%(app_label)s_%(class)s_related")

    class Meta:
        abstract = True
        ordering = ['created', 'age']

class Student(CommonInfo):
    home_group = models.CharField(max_length=5)

class Student2(CommonInfo):
    home_group = models.CharField(max_length=5)


class Student3(Student2):
    home_group3 = models.CharField(max_length=5)
