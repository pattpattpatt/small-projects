import bs4

class Soup_Interface:
    def __init__(self, soup_object):
        self.soup = soup_object

    def get_title(self):
        title = self.soup.find(class_="title").text
        return title

    def get_article_body(self):
        body = self.soup.find(class_="body").find_all()
        return body

    def get_topics(self):
        topics = self.soup.find(class_="categories").text
        return topics

    def get_teaser(self):
        #Is there a consistent definition of a teaser
        pass

    def get_author(self):
        author = self.soup.find(class_="author").text
        author = author[3:len(author)] #gets rid of leading "By "
        return author

    def get_publish_date(self):
        pub_date = self.soup.find(class_="pubdate").text
        return pub_date

    def get_img_list(self):
        #get list of image urls
        img_list = self.soup.find(class_="body").select("img")

        images = []
        for image_url in img_list:
            image = image_url["src"][2:len(image_url["src"])]#strips leading "//" from img url
            images.append(image)
        return images
