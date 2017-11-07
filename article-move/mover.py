from bs4 import BeautifulSoup
from urllib.request import urlopen
import csv
import sys
from soup_interface import Soup_Interface

def write_to_csv():
    pass

#import urls from csv
def import_urls(source_path):
    #open link to csv file
    with open(source_path,newline='') as url_file:
        #get the csv reader
        file_reader = csv.reader(url_file)

        output_list = []

        #get all paths
        for row in file_reader:
            temp = row[0] + row[1]
            output_list.append(temp)

        return output_list


#get html from url
def source_html(url):
    #create request
    request = urlopen(url)

    #get html and decode to string
    html_bytes = request.read();
    html_string = html_bytes.decode("utf8")

    #close connection
    request.close()

    return html_string

#create soup from html
def create_soup(url):
    html = source_html(url)

    soup = BeautifulSoup(html)

    return soup

def main():
    input_path = sys.argv[1]

    urls = import_urls(input_path)
    #print(urls[900])
    soup = create_soup(urls[int(sys.argv[2])])

    se = Soup_Interface(soup)

    print(se.get_title())
    print(se.get_author())
    print(se.get_topics())
    print(se.get_publish_date())
    print(se.get_article_body())
    print(se.get_img_list())


if __name__ == "__main__":
    main()
