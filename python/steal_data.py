#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2, lxml.etree, httplib, sys, locale

URL = 'http://www.stats.gov.cn/tjbz/cxfldm/2010/'


def _print(name, code, lv=0, page=''):
    print(u"INSERT INTO CHINA (CODE, NAME, LV) VALUES('%s', '%s', %d);" % (code, name, lv))

def steal_and_print():
    parserProvince()

def _to_etree(page):
    try:
        f = urllib2.urlopen(page)
    except httplib.BadStatusLine:
        return None
    except urllib2.HTTPError:
        return None
    return lxml.etree.HTML(f.read())

def _to_page_and_print(l, lv=0):
    link = l.get('href').split('/')[-1]
    _page = '/'.join([link[i*2:i*2+2] for i in range(0,lv) ] + [link])
    _print(l.text, link.split('.')[0], lv, _page)
    return _page


def parserProvince():
    tree = _to_etree(URL+'index.html') 
    links = tree.xpath("//table[@class='provincetable']/tr[@class='provincetr']/td/a")
    for l in links:
        _print(l.text, l.get('href').split('.')[0], 0)
        parserCity(l.get('href'))

def parserCity(page):
    tree = _to_etree(URL+page)
    if tree is not None:
        links = tree.xpath("//tr[@class='citytr']/td/a")
        for i in range(1, len(links), 2):
            _page = _to_page_and_print(links[i], 1)
            parserCountry(_page)

def parserCountry(page):
    tree = _to_etree(URL+page)
    if tree is not None:
        links = tree.xpath("//tr[@class='countytr']/td/a")
        for i in range(1, len(links), 2):
            _page = _to_page_and_print(links[i], 2)
            parserTown(_page)


def parserTown(page):
    tree = _to_etree(URL+page)
    if tree is not None:
        links = tree.xpath("//tr[@class='towntr']/td/a")
        for i in range(1, len(links), 2):
            _page = _to_page_and_print(links[i], 3)
#        parserVillage(_page)

def parserVillage(page):
    tree = _to_etree(URL+page)
    if tree:
        tds = tree.xpath("//tr[@class='villagetr']/td")
        for i in range(0, len(tds), 3):
            if i + 3 < len(tds):
                _print(tds[i+2].text, tds[i].text, 4)

if __name__ == '__main__':
    steal_and_print()

